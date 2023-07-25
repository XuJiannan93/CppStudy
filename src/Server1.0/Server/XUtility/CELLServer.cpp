#include "CELLTime.h"
#include "CELLServer.h"

CELLServer::CELLServer(int id)
{
	_id = id;
	_pNetEvent = nullptr;
	_taskServer.serverID = id;
	_maxSock = -1;
	//FD_ZERO(&_fdRead);
	//FD_ZERO(&_fdWrite);
	//FD_ZERO(&_fdExc);
	//FD_ZERO(&_fdRead_bak);
	_fdRead.Zero();
	_fdWrite.Zero();
	_fdRead_bak.Zero();
	_old_time = CELLTime::GetNowInMilliSec();
}

CELLServer::~CELLServer()
{
	CELLLog_Info("CELLServer[%d]::~CELLServer() 1", _id);
	Close();
	CELLLog_Info("CELLServer[%d]::~CELLServer() 2", _id);
}

void CELLServer::SetEventObj(INetEvent* evt)
{
	_pNetEvent = evt;
}

void CELLServer::Start()
{
	_taskServer.Start();

	_thread.Start(
		nullptr,
		[this](CELLThread* pThread) {
			OnRun(pThread);
		},
		[this](CELLThread* pThread) {
			_clients.clear();
			_clientsBuff.clear();
		});
}

void CELLServer::OnRun(CELLThread* pThread)
{
	while (pThread->isRun())
	{
		if (_clientsBuff.size() > 0)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			for (auto pClient : _clientsBuff)
			{
				if (_pNetEvent)
					_pNetEvent->OnNetJoin(pClient);
				_clients[pClient->sockfd()] = pClient;
			}
			_clientsBuff.clear();
			_clients_changed = true;
		}

		if (_clients.empty())
		{
			CELLThread::Sleep(1);
			_old_time = CELLTime::GetNowInMilliSec();

			continue;
		}

		_CheckTime();
		if (_DoSelect())
		{
			_DoMsg();
			continue;
		}

		_DoMsg();
		pThread->Exit();
		break;
	}

	CELLLog_Info("CELLServer[%d]::OnRun() exit", _id);
}

void CELLServer::_OnClientLeave(CELLClientPtr pClient)
{
	if (_pNetEvent)
		_pNetEvent->OnNetLeave(pClient);
	_clients_changed = true;
}

bool CELLServer::_DoSelect()
{
	if (_clients_changed)
	{
		_clients_changed = false;
		//FD_ZERO(&_fdRead);
		_fdRead.Zero();
		_maxSock = _clients.begin()->second->sockfd();
		for (auto iter : _clients)
		{
			//FD_SET(iter.second->sockfd(), &_fdRead);
			_fdRead.Set(iter.second->sockfd());
			if (_maxSock < iter.second->sockfd())
				_maxSock = iter.second->sockfd();
			//memcpy(&_fdRead_bak, _fdRead.Get(), sizeof(fd_set));
			_fdRead_bak.Copy(_fdRead);
		}
	}
	else
	{
		//memcpy(&_fdRead, _fdRead_bak.Get(), sizeof(fd_set));
		_fdRead.Copy(_fdRead_bak);
	}

	bool bNeedWrite = false;
	//FD_ZERO(&_fdWrite);
	_fdWrite.Zero();
	for (auto iter : _clients)
	{
		if (iter.second->NeedWrite())
		{
			bNeedWrite = true;
			//FD_SET(iter.second->sockfd(), &_fdWrite);
			_fdWrite.Set(iter.second->sockfd());
		}
	}

	timeval t = { 0, 1 };
	int ret = 0;
	if (bNeedWrite)
		ret = select(_maxSock + 1, _fdRead.Get(), _fdWrite.Get(), nullptr, &t);
	else
		ret = select(_maxSock + 1, _fdRead.Get(), nullptr, nullptr, &t);

	if (ret < 0)
	{
		CELLLog_Info("select task end. ");
		return false;
	}
	else if (ret == 0) return true;

	_ReadData();
	_WriteData();
	//_WriteData(_fdExc);

	return true;
}

void CELLServer::_DoMsg()
{
	for (auto itr : _clients)
	{
		auto client = itr.second;
		while (client->HasMsg())
		{
			OnNetMsg(client, client->front_msg());
			client->pop_front_msg();
		}
	}
}

void CELLServer::_ReadData()
{
#ifdef _WIN32
	auto pfdset = _fdRead.Get();
	for (int n = 0; n < pfdset->fd_count; n++)
	{
		auto iter = _clients.find(pfdset->fd_array[n]);

		if (RecvData(iter->second) == SOCKET_ERROR)
		{
			_OnClientLeave(iter->second);
			_clients.erase(iter);
		}
	}
#else
	std::vector<ClientSocketPtr> temp;
	for (auto iter : _clients)
	{
		//if (FD_ISSET(iter.second->sockfd(), &fdRead))
		if (_fdRead.IsSet(iter.second->sockfd()))
		{
			if (RecvData(iter.second) == SOCKET_ERROR)
			{
				_OnClientLeave(iter->second);
				temp.push_back(iter.second);
			}
		}
	}
	for (auto pClient : temp)
	{
		_clients.erase(pClient->sockfd());
	}
#endif
}

void CELLServer::_WriteData()
{
#ifdef _WIN32
	auto pfdset = _fdWrite.Get();
	for (int n = 0; n < pfdset->fd_count; n++)
	{
		auto iter = _clients.find(pfdset->fd_array[n]);
		if (iter != _clients.end())
		{
			if (iter->second->SendDataImmediately() == SOCKET_ERROR)
			{
				_OnClientLeave(iter->second);
				_clients.erase(iter);
			}
		}
	}
#else
	for (auto iter = _clients.begin(); iter != _clients.end();)
	{
		if (iter->second->NeetWrite() == false)
		{
			iter++;
			continue;
		}

		//if (FD_ISSET(iter->second->sockfd(), &fdWrite) == false)
		if (_fdWrite.IsSet(iter->second->sockfd()) == false)
		{
			iter++;
			continue;
		}

		if (iter->second->SendDataImmediately() == SOCKET_ERROR)
		{
			_OnClientLeave(iter->second);
			auto iterOld = iter;
			iter++;
			_clients.erase(iterOld);
			continue;
		}

		iter++;
	}
#endif
}

void CELLServer::_CheckTime()
{
	auto tNow = CELLTime::GetNowInMilliSec();
	auto dt = tNow - _old_time;
	_old_time = tNow;

	for (auto iter = _clients.begin(); iter != _clients.end();)
	{
		if (iter->second->CheckHeart(dt))
		{
			if (_pNetEvent)
				_pNetEvent->OnNetLeave(iter->second);
			_clients_changed = true;
			auto iterOld = iter++;
			_clients.erase(iterOld);
			continue;
		}

		//iter->second->CheckSend(dt);
		iter++;
	}
}

int CELLServer::RecvData(CELLClientPtr client)
{
	int len = client->RecvData();
	_pNetEvent->OnNetRecv(client);
	return len;
}

void CELLServer::OnNetMsg(CELLClientPtr& pClient, netmsg_DataHeader* header)
{
	_pNetEvent->OnNetMsg(this, pClient, header);
}

void CELLServer::Close()
{

	CELLLog_Info("CELLServer[%d]::Close() 1", _id);

	_taskServer.Close();
	_thread.Close();

	CELLLog_Info("CELLServer[%d]::Close() 2", _id);
}

void CELLServer::addClient(CELLClientPtr& pClient)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_clientsBuff.push_back(pClient);
}

void CELLServer::AddSendTask(CELLClientPtr& pClient, DataHeaderPtr& header)
{
	_taskServer.AddTask([pClient, header]() {
		if (pClient->SendData(header) == SOCKET_ERROR)
		{
			//send buf full, msg not send
			CELLLog_Info("socket[%d] send full", (int)pClient->sockfd());
		}});
}
