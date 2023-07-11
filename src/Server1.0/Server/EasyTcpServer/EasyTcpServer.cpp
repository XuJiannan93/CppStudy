#include "EasyTcpServer.h"

EasyTcpServer::EasyTcpServer()
{
	_sock = INVALID_SOCKET;
	_recvCount = 0;
	_clientCount = 0;
	_msgCount = 0;
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

void EasyTcpServer::AddClientToCellServer(ClientSocketPtr& pclient)
{
	//_clients.push_back(pclient);

	auto pMinServer = _cellServers[0];
	for (auto pCellServer : _cellServers)
	{
		if (pMinServer->GetClientCount() > pCellServer->GetClientCount())
			pMinServer = pCellServer;
	}

	pMinServer->addClient(pclient);
	//OnNetJoin(pclient);
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
		//ClientSocketPtr ptr = std::make_shared<ClientSocket>(client);
		std::shared_ptr<CellClient> ptr(new CellClient(client));
		AddClientToCellServer(ptr);
	}

	return client;
}

void EasyTcpServer::Start(int nCellServer)
{
	for (int n = 0; n < nCellServer; n++)
	{
		std::shared_ptr<CellServer> ser(new CellServer(n + 1));
		_cellServers.push_back(ser);
		ser->SetEventObj(this);
		ser->Start();
	}
}

bool EasyTcpServer::OnRun()
{
	if (IsRun() == false) return false;

	time4msg();

	fd_set fdRead;
	FD_ZERO(&fdRead);
	FD_SET(_sock, &fdRead);

	timeval t = { 0, 0 };
	int ret = select(_sock + 1, &fdRead, nullptr, nullptr, &t);
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

	return true;
}

void EasyTcpServer::SendDataToAll(netmsg_DataHeader* header)
{
	//for (int n = (int)_clients.size() - 1; n >= 0; n--)
	//{
	//	SendData(_clients[n]->sockfd(), header);
	//}
}

int EasyTcpServer::RecvData(ClientSocketPtr& client)
{
	int nLen = (int)recv(client->sockfd(), _szRecv, RECV_BUFF_SIZE, 0);
	if (nLen <= 0)
	{
		printf("client[%d] broken.\n", (int)client->sockfd());
		return -1;
	}

	memcpy(client->msgBuf() + client->getLastPos(), _szRecv, nLen);
	client->setLastPos(client->getLastPos() + nLen);

	while (client->getLastPos() >= sizeof(netmsg_DataHeader))
	{
		netmsg_DataHeader* header = (netmsg_DataHeader*)client->msgBuf();
		if (client->getLastPos() < header->len)
			break;

		int pos = client->getLastPos() - header->len;
		//time4msg(/*client->sockfd(), header*/);
		memcpy(client->msgBuf(), client->msgBuf() + header->len, pos);
		client->setLastPos(pos);
	}

	return 0;
}

void EasyTcpServer::time4msg(/*SOCKET _client, DataHeader* header*/)
{
	auto t1 = _tTime.GetElapsedSecond();

	if (t1 >= 1.0)
	{
		printf("thread<%d>,time<%f>,socket<%d>,client<%d>,recv<%d>,msg<%d>\n", (int)_cellServers.size(), t1, (int)_sock, _clientCount.load(), (int)(_recvCount / t1), (int)(_msgCount / t1));
		_recvCount = 0;
		_msgCount = 0;
		_tTime.Update();
	}
}

void EasyTcpServer::OnNetJoin(ClientSocketPtr& pClient)
{
	_clientCount++;
}

void EasyTcpServer::OnNetLeave(ClientSocketPtr& pClient)
{
	_clientCount--;
}

void EasyTcpServer::OnNetMsg(CellServer* pCellServer, ClientSocketPtr& pClient, netmsg_DataHeader* header)
{
	_msgCount++;
}

void EasyTcpServer::OnNetRecv(ClientSocketPtr& pClient)
{
	_recvCount++;
}

void EasyTcpServer::Close()
{
	printf("EasyTcpServer::Close() 1\n");

	if (_sock == INVALID_SOCKET) return;

	_cellServers.clear();

#ifdef _WIN32
	closesocket(_sock);
	WSACleanup();
#else
	close(_sock);
#endif

	_sock = INVALID_SOCKET;

	printf("EasyTcpServer::Close() 2\n");
}
