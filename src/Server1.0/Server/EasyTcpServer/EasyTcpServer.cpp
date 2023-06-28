#include "EasyTcpServer.h"

EasyTcpServer::EasyTcpServer()
{
	_sock = INVALID_SOCKET;
}

EasyTcpServer::~EasyTcpServer()
{
	Close();
}

SOCKET EasyTcpServer::InitSocket()
{
#ifdef _WIN32
	WORD ver = MAKEWORD(2, 2);
	WSADATA data;
	WSAStartup(ver, &data);
#endif

	if (_sock != INVALID_SOCKET)
	{
		printf("close old connection. \n");
		Close();
	}

	_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sock == INVALID_SOCKET)
	{
		printf("[ERROR]create socket failed.\n");
	}
	else
		printf("create socket succeed.\n");

	return _sock;
}

int EasyTcpServer::Bind(const char* ip, unsigned short port)
{
	if (_sock == INVALID_SOCKET)
		InitSocket();

	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(port); // host to net unsigned short
	//_sin.sin_addr.S_un.S_addr = INADDR_ANY;		//inet_addr("127.0.0.1");
	if (ip == nullptr)
	{
#ifdef _WIN32
		_sin.sin_addr.S_un.S_addr = INADDR_ANY;
#else
		_sin.sin_addr.s_addr = INADDR_ANY;
#endif
	}
	else
	{
#ifdef _WIN32
		_sin.sin_addr.S_un.S_addr = inet_addr(ip);
#else 
		_sin.sin_addr.s_addr = inet_addr(ip);
#endif
	}
	int ret = bind(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
	if (ret == SOCKET_ERROR)
		printf("[ERROR]bind[%d] socket failed.\n", port);
	else
		printf("bind socket succeed.\n");
	return ret;
}

int EasyTcpServer::Listen(int n)
{
	int ret = listen(_sock, n);
	if (ret == SOCKET_ERROR)
		printf("[ERROR]listen[%d] socket failed.\n", (int)_sock);
	else
		printf("listen socket[%d] succeed.\n", (int)_sock);
	return ret;
}

SOCKET EasyTcpServer::Accept()
{
	sockaddr_in clientAddr = {};
	int nAddrLen = sizeof(clientAddr);
	SOCKET client = INVALID_SOCKET;

	client = accept(_sock, (sockaddr*)&clientAddr, (socklen_t*)&nAddrLen);
	if (client == INVALID_SOCKET)
		printf("[ERROR]accept socket failed.\n");
	else
	{
		NewUserJoin join = {};
		SendDataToAll(&join);

		_clients.push_back(new ClientSocket(client));
		//printf("new client[%d][IP : %s] linked in.\n", (int)client, inet_ntoa(clientAddr.sin_addr));
	}

	return client;
}

bool EasyTcpServer::OnRun()
{
	if (IsRun() == false) return false;

	fd_set fdRead;
	fd_set fdWrite;
	fd_set fdExc;

	FD_ZERO(&fdRead);
	FD_ZERO(&fdWrite);
	FD_ZERO(&fdExc);

	FD_SET(_sock, &fdRead);
	FD_SET(_sock, &fdWrite);
	FD_SET(_sock, &fdExc);

	SOCKET _maxSock = _sock;
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
	int ret = select(_maxSock + 1, &fdRead, &fdWrite, &fdExc, &t);
	if (ret < 0)
	{
		printf("select task end. \n");
		Close();
		return false;
	}

	if (FD_ISSET(_sock, &fdRead))
	{
		FD_CLR(_sock, &fdRead);
		Accept();
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
					delete _clients[n];
					_clients.erase(iter);
				}
			}
		}
	}

	return true;
}

int EasyTcpServer::SendData(SOCKET client, DataHeader* header)
{
	if (IsRun() && header)
	{
		return send(client, (const char*)header, header->len, 0);
	}
	return SOCKET_ERROR;
}

void EasyTcpServer::SendDataToAll(DataHeader* header)
{
	for (int n = (int)_clients.size() - 1; n >= 0; n--)
	{
		SendData(_clients[n]->sockfd(), header);
	}
}

int EasyTcpServer::RecvData(ClientSocket* client)
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

void EasyTcpServer::OnNetMsg(SOCKET _client, DataHeader* header)
{
	switch (header->cmd)
	{

	case CMD_LOGIN:
	{
		Login* login = (Login*)header;
		printf("recv<%d> cmd len :login [%s][%s][%d] \n", (int)_client, login->username, login->password, header->len);

		LoginResult rst;
		send(_client, (char*)&rst, sizeof(LoginResult), 0);
	}
	break;

	case CMD_LOGOUT:
	{
		Logout* logout = (Logout*)header;
		printf("recv<%d> cmd len :logout [%s][%d] \n", (int)_client, logout->username, header->len);

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

void EasyTcpServer::Close()
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
