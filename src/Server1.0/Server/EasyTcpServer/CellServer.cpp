#include "CellServer.h"

CellServer::CellServer(SOCKET sock)
{
	_sock = sock;
	_pthread = nullptr;
	recvCount = 0;
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
	_pthread = new  std::thread(std::mem_fun(&CellServer::OnRun), this);
}

void CellServer::OnRun()
{
	while (IsRun())
	{
		if (_clientsBuff.size() > 0)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			for (auto pClient : _clientsBuff)
			{
				_clients.push_back(pClient);
			}
			_clientsBuff.clear();
		}

		if (_clients.empty())
		{
			std::chrono::milliseconds t(1);
			std::this_thread::sleep_for(t);

			continue;
		}

		fd_set fdRead;
		//fd_set fdWrite;
		//fd_set fdExc;

		FD_ZERO(&fdRead);
		//FD_ZERO(&fdWrite);
		//FD_ZERO(&fdExc);

		//FD_SET(_sock, &fdRead);
		//FD_SET(_sock, &fdWrite);
		//FD_SET(_sock, &fdExc);

		SOCKET _maxSock = _clients[0]->sockfd();
		for (int n = (int)_clients.size() - 1; n >= 0; n--)
		{
			FD_SET(_clients[n]->sockfd(), &fdRead);
			if (_maxSock < _clients[n]->sockfd())
				_maxSock = _clients[n]->sockfd();
		}

		for (int n = (int)_clients.size() - 1; n >= 0; n--)
		{
			FD_SET(_clients[n]->sockfd(), &fdRead);
		}

		timeval t = { 1, 0 };
		int ret = select(_maxSock + 1, &fdRead, nullptr, nullptr, nullptr);
		if (ret < 0)
		{
			printf("select task end. \n");
			Close();
			return;
		}

		for (int n = (int)_clients.size() - 1; n >= 0; n--)
		{
			if (FD_ISSET(_clients[n]->sockfd(), &fdRead))
			{
				if (RecvData(_clients[n]) == -1)
				{
					auto iter = _clients.begin() + n;
					if (iter != _clients.end())
					{
						if (_pNetEvent)
							_pNetEvent->OnLeave(_clients[n]);

						delete _clients[n];
						_clients.erase(iter);
					}
				}
			}
		}
	}
}

int CellServer::RecvData(ClientSocket* client)
{
	int nLen = (int)recv(client->sockfd(), _szRecv, RECV_BUFF_SIZE, 0);
	if (nLen <= 0)
	{
		printf("client[%d] broken.\n", (int)client);
		return -1;
	}

	memcpy(client->msgBuf() + client->getLastPos(), _szRecv, nLen);
	client->setLastPos(client->getLastPos() + nLen);

	while (client->getLastPos() >= sizeof(DataHeader))
	{
		DataHeader* header = (DataHeader*)client->msgBuf();
		if (client->getLastPos() < header->len)
			break;

		int pos = client->getLastPos() - header->len;
		OnNetMsg(client->sockfd(), header);
		memcpy(client->msgBuf(), client->msgBuf() + header->len, pos);
		client->setLastPos(pos);
	}

	return 0;
}

void CellServer::OnNetMsg(SOCKET _client, DataHeader* header)
{
	recvCount++;

	//_pNetEvent->OnNetMsg(_client, header);

	//auto t1 = _tTime.GetElapsedSecond();
	//if (t1 >= 1.0)
	//{
	//	printf("time<%1f>,socket<%d>,clients<%d>,recvCount<%d>\n", t1, _sock, _clients.size(), _recvCount);
	//	_recvCount = 0;
	//	_tTime.Update();
	//}

	switch (header->cmd)
	{

	case CMD_LOGIN:
	{
		Login* login = (Login*)header;
		//printf("recv<%d> cmd len :login [%s][%s][%d] \n", (int)_client, login->username, login->password, header->len);

		LoginResult rst;
		send(_client, (char*)&rst, sizeof(LoginResult), 0);
	}
	break;

	case CMD_LOGOUT:
	{
		Logout* logout = (Logout*)header;
		//printf("recv<%d> cmd len :logout [%s][%d] \n", (int)_client, logout->username, header->len);

		LogoutResult rst;
		send(_client, (char*)&rst, sizeof(LogoutResult), 0);
	}
	break;

	case CMD_ERROR:
		printf("Recv Undefined Message! \n");
		break;


	default:
	{
		DataHeader errormsg = {};
		send(_client, (char*)&errormsg, errormsg.len, 0);
	}
	break;
	}

}

void CellServer::Close()
{
#ifdef _WIN32
	for (int n = (int)_clients.size() - 1; n >= 0; n--)
	{
		closesocket(_clients[n]->sockfd());
		delete _clients[n];
	}
	closesocket(_sock);
#else
	for (int n = (int)g_clients.size() - 1; n >= 0; n--)
	{
		close(g_clients[n]->sockfd());
		delete _clients[n];
	}
	close(_sock);
#endif

	_clients.clear();

#ifdef _WIN32
	WSACleanup();
#endif
}

void CellServer::addClient(ClientSocket* pClient)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_clientsBuff.push_back(pClient);
}
