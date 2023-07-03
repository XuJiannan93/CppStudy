#include "CellServer.h"

CellServer::CellServer(SOCKET sock)
{
	_sock = sock;
	//recvCount = 0;
	_pNetEvent = nullptr;
}

CellServer::~CellServer()
{
	Close();
	_sock = INVALID_SOCKET;
}

void CellServer::SetEventObj(INetEvent* evt)
{
	_pNetEvent = evt;
}

void CellServer::Start()
{
	_thread = std::thread(std::mem_fn(&CellServer::OnRun), this);
}


void CellServer::OnRun()
{
	_clients_changed = true;
	while (IsRun())
	{
		if (_clientsBuff.size() > 0)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			for (auto pClient : _clientsBuff)
			{
				//_clients.push_back(pClient);
				_clients[pClient->sockfd()] = pClient;
			}
			_clientsBuff.clear();
			_clients_changed = true;
		}

		if (_clients.empty())
		{
			std::chrono::milliseconds t(1);
			std::this_thread::sleep_for(t);

			continue;
		}

		fd_set fdRead;
		FD_ZERO(&fdRead);

		if (_clients_changed)
		{
			_clients_changed = false;
			_maxSock = _clients.begin()->second->sockfd();
			for (auto iter : _clients)
			{
				FD_SET(iter.second->sockfd(), &fdRead);
				if (_maxSock < iter.second->sockfd())
					_maxSock = iter.second->sockfd();
				memcpy(&_fdRead_bak, &fdRead, sizeof(fd_set));
			}
		}
		else
		{
			memcpy(&fdRead, &_fdRead_bak, sizeof(fd_set));
		}

		timeval t = { 0, 0 };
		int ret = select(_maxSock + 1, &fdRead, nullptr, nullptr, nullptr);
		if (ret < 0)
		{
			printf("select task end. \n");
			Close();
			return;
		}
		else if (ret == 0) continue;

#ifdef _WIN32
		for (int n = 0; n < fdRead.fd_count; n++)
		{
			auto iter = _clients.find(fdRead.fd_array[n]);

			if (RecvData(iter->second) == -1)
			{
				_clients_changed = true;

				if (_pNetEvent)
					_pNetEvent->OnNetLeave(iter->second);

				delete iter->second;
				_clients.erase(iter->first);
			}
		}
#else
		std::vector<ClientSocket*> temp;
		for (auto iter : _clients)
		{
			if (FD_ISSET(iter.second->sockfd(), &fdRead))
			{
				if (RecvData(iter.second) == -1)
				{
					if (_pNetEvent)
						_pNetEvent->OnNetLeave(iter.second);

					_clients_changed = true;
					temp.push_back(iter.second);
				}
			}
		}
		for (auto pClient : temp)
		{
			_clients.erase(pClient->sockfd());
			delete pClient;
		}
#endif

	}
}

//int CellServer::SendData(SOCKET client, DataHeader* header)
//{
//	if (IsRun() && header)
//	{
//		return send(client, (const char*)header, header->len, 0);
//	}
//	return SOCKET_ERROR;
//}

int CellServer::RecvData(ClientSocket* client)
{
	char* szRecv = client->msgBuf() + client->getLastPos();
	int nLen = (int)recv(client->sockfd(), szRecv, (RECV_BUFF_SIZE * 5) - client->getLastPos(), 0);
	_pNetEvent->OnNetRecv(client);
	if (nLen <= 0)
	{
		//printf("client[%d] broken.\n", (int)client);
		return -1;
	}

	//memcpy(client->msgBuf() + client->getLastPos(), _szRecv, nLen);
	client->setLastPos(client->getLastPos() + nLen);

	while (client->getLastPos() >= sizeof(DataHeader))
	{
		DataHeader* header = (DataHeader*)client->msgBuf();
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

void CellServer::OnNetMsg(ClientSocket* pClient, DataHeader* header)
{
	//recvCount++;

	_pNetEvent->OnNetMsg(pClient, header);

	//auto t1 = _tTime.GetElapsedSecond();
	//if (t1 >= 1.0)
	//{
	//	printf("time<%1f>,socket<%d>,clients<%d>,recvCount<%d>\n", t1, _sock, _clients.size(), _recvCount);
	//	_recvCount = 0;
	//	_tTime.Update();
	//}


}

void CellServer::Close()
{
#ifdef _WIN32

	for (auto iter : _clients)
	{
		closesocket(iter.second->sockfd());
		delete iter.second;
	}
	closesocket(_sock);
	//WSACleanup();
#else
	for (auto iter : _clients)
	{
		close(iter.second->sockfd());
		delete iter.second;
}
	close(_sock);
#endif

	_clients.clear();
}

void CellServer::addClient(ClientSocket* pClient)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_clientsBuff.push_back(pClient);
}
