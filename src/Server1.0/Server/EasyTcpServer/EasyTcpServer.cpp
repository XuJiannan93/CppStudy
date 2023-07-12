#include <signal.h>

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
#else
	signal(SIGPIPE, SIG_IGN);
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

void EasyTcpServer::AddClientToCELLServer(CELLClientPtr& pclient)
{
	//_clients.push_back(pclient);

	auto pMinServer = _CELLServers[0];
	for (auto pCELLServer : _CELLServers)
	{
		if (pMinServer->GetClientCount() > pCELLServer->GetClientCount())
			pMinServer = pCELLServer;
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
		std::shared_ptr<CELLClient> ptr(new CELLClient(client));
		AddClientToCELLServer(ptr);
	}

	return client;
}

void EasyTcpServer::Start(int nCELLServer)
{
	_thread.Start(
		[this, nCELLServer](CELLThread* pThread) {
			for (int n = 0; n < nCELLServer; n++)
			{
				std::shared_ptr<CELLServer> ser(new CELLServer(n + 1));
				_CELLServers.push_back(ser);
				ser->SetEventObj(this);
				ser->Start();
			}
		},
		[this](CELLThread* pThread)
		{
			OnRun(pThread);
		},
		[this](CELLThread* pThread)
		{
			_CELLServers.clear();
		});
}

void EasyTcpServer::OnRun(CELLThread* pThread)
{
	while (pThread->isRun())
	{
		time4msg();

		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET(_sock, &fdRead);

		timeval t = { 0, 0 };
		int ret = select(_sock + 1, &fdRead, nullptr, nullptr, &t);
		if (ret < 0)
		{
			printf("select task end. \n");
			pThread->Exit();
			break;
		}

		if (FD_ISSET(_sock, &fdRead))
		{
			FD_CLR(_sock, &fdRead);
			Accept();
		}
	}
}

void EasyTcpServer::SendDataToAll(netmsg_DataHeader* header)
{
	//for (int n = (int)_clients.size() - 1; n >= 0; n--)
	//{
	//	SendData(_clients[n]->sockfd(), header);
	//}
}

void EasyTcpServer::time4msg(/*SOCKET _client, DataHeader* header*/)
{
	auto t1 = _tTime.GetElapsedSecond();

	if (t1 >= 1.0)
	{
		printf("thread<%d>,time<%f>,socket<%d>,client<%d>,recv<%d>,msg<%d>\n", (int)_CELLServers.size(), t1, (int)_sock, _clientCount.load(), (int)(_recvCount / t1), (int)(_msgCount / t1));
		_recvCount = 0;
		_msgCount = 0;
		_tTime.Update();
	}
}

void EasyTcpServer::OnNetJoin(CELLClientPtr& pClient)
{
	_clientCount++;
}

void EasyTcpServer::OnNetLeave(CELLClientPtr& pClient)
{
	_clientCount--;
}

void EasyTcpServer::OnNetMsg(CELLServer* pCELLServer, CELLClientPtr& pClient, netmsg_DataHeader* header)
{
	_msgCount++;
}

void EasyTcpServer::OnNetRecv(CELLClientPtr& pClient)
{
	_recvCount++;
}

void EasyTcpServer::Close()
{
	printf("EasyTcpServer::Close() 1\n");
	_thread.Close();

	if (_sock == INVALID_SOCKET) return;

#ifdef _WIN32
	closesocket(_sock);
	WSACleanup();
#else
	close(_sock);
#endif

	_sock = INVALID_SOCKET;

	printf("EasyTcpServer::Close() 2\n");
}
