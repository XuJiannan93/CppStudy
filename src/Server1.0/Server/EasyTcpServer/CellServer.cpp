#include "CELLTime.h"
#include "CELLServer.h"

CELLServer::CELLServer(int id)
{
	_id = id;
	_pNetEvent = nullptr;
	_taskServer.serverID = id;
	_maxSock = -1;
	FD_ZERO(&_fdRead);
	FD_ZERO(&_fdWrite);
	//FD_ZERO(&_fdExc);
	FD_ZERO(&_fdRead_bak);
	_old_time = CELLTime::GetNowInMilliSec();
}

CELLServer::~CELLServer()
{
	printf("CELLServer[%d]::~CELLServer() 1\n", _id);
	Close();
	printf("CELLServer[%d]::~CELLServer() 2\n", _id);
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
			std::chrono::milliseconds t(1);
			std::this_thread::sleep_for(t);

			_old_time = CELLTime::GetNowInMilliSec();

			continue;
		}

		FD_ZERO(&_fdWrite);
		//FD_ZERO(&_fdExc);

		if (_clients_changed)
		{
			_clients_changed = false;
			FD_ZERO(&_fdRead);
			_maxSock = _clients.begin()->second->sockfd();
			for (auto iter : _clients)
			{
				FD_SET(iter.second->sockfd(), &_fdRead);
				if (_maxSock < iter.second->sockfd())
					_maxSock = iter.second->sockfd();
				memcpy(&_fdRead_bak, &_fdRead, sizeof(fd_set));
			}
		}
		else
		{
			memcpy(&_fdRead, &_fdRead_bak, sizeof(fd_set));
		}

		memcpy(&_fdWrite, &_fdRead_bak, sizeof(fd_set));
		//memcpy(&_fdExc, &_fdRead_bak, sizeof(fd_set));

		timeval t = { 0, 0 };
		int ret = select(_maxSock + 1, &_fdRead, &_fdWrite, nullptr, &t);
		if (ret < 0)
		{
			printf("select task end. \n");
			pThread->Exit();
			break;
		}
		/*	else if (ret == 0) continue;*/

		_ReadData(_fdRead);
		_WriteData(_fdWrite);
		//_WriteData(_fdExc);

		_CheckTime();
	}

	printf("CELLServer[%d]::OnRun() exit\n", _id);
}

void CELLServer::_OnClientLeave(CELLClientPtr pClient)
{
	if (_pNetEvent)
		_pNetEvent->OnNetLeave(pClient);
	_clients_changed = true;
}

void CELLServer::_ReadData(fd_set& fdRead)
{
#ifdef _WIN32
	for (int n = 0; n < fdRead.fd_count; n++)
	{
		auto iter = _clients.find(fdRead.fd_array[n]);

		if (RecvData(iter->second) == -1)
		{
			_OnClientLeave(iter->second);
			_clients.erase(iter);
		}
	}
#else
	std::vector<ClientSocketPtr> temp;
	for (auto iter : _clients)
	{
		if (FD_ISSET(iter.second->sockfd(), &fdRead))
		{
			if (RecvData(iter.second) == -1)
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

void CELLServer::_WriteData(fd_set& fdWrite)
{
#ifdef _WIN32
	for (int n = 0; n < fdWrite.fd_count; n++)
	{
		auto iter = _clients.find(fdWrite.fd_array[n]);
		if (iter != _clients.end())
		{
			if (iter->second->SendDataImmediately() == -1)
			{
				_OnClientLeave(iter->second);
				_clients.erase(iter);
			}
		}
	}
#else
	std::vector<ClientSocketPtr> temp;
	for (auto iter : _clients)
	{
		if (FD_ISSET(iter.second->sockfd(), &fdRead))
		{
			if (RecvData(iter.second) == -1)
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
	char* szRecv = client->msgBuf() + client->getLastPos();
	int nLen = (int)recv(client->sockfd(), szRecv, RECV_BUFF_SIZE - client->getLastPos(), 0);
	_pNetEvent->OnNetRecv(client);
	if (nLen <= 0)
	{
		//printf("client[%d] broken.\n", (int)client);
		return -1;
	}

	//memcpy(client->msgBuf() + client->getLastPos(), _szRecv, nLen);
	client->setLastPos(client->getLastPos() + nLen);

	while (client->getLastPos() >= sizeof(netmsg_DataHeader))
	{
		netmsg_DataHeader* header = (netmsg_DataHeader*)client->msgBuf();
		if (client->getLastPos() < header->len)
			break;

		int pos = client->getLastPos() - header->len;
		OnNetMsg(client, header);
		memcpy(client->msgBuf(), client->msgBuf() + header->len, pos);
		//memcpy(client->msgBuf(), szRecv + header->len, pos);
		client->setLastPos(pos);
	}

	return 0;
}

void CELLServer::OnNetMsg(CELLClientPtr& pClient, netmsg_DataHeader* header)
{
	_pNetEvent->OnNetMsg(this, pClient, header);
}

void CELLServer::Close()
{

	printf("CELLServer[%d]::Close() 1\n", _id);

	_taskServer.Close();
	_thread.Close();

	printf("CELLServer[%d]::Close() 2\n", _id);
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
			printf("socket[%d] send full\n", pClient->sockfd());
		}});
}
