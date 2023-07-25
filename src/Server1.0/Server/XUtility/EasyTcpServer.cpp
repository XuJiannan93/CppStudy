#include <signal.h>

#include "EasyTcpServer.h"
#include "CELLNetwork.h"
#include "CELLConfig.h"

EasyTcpServer::EasyTcpServer()
{
	_sock = INVALID_SOCKET;
	fdRead.Zero();
	_recvCount = 0;
	_clientCount = 0;
	_msgCount = 0;
	_nSendBufSize = CELLConfig::Instance().GetInt("nSendBufSize", SEND_BUFF_SIZE);
	_nRecvBufSize = CELLConfig::Instance().GetInt("nRecvBufSize", SEND_BUFF_SIZE);
	_nMaxClient = CELLConfig::Instance().GetInt("nMaxClient", FD_SETSIZE);
}

EasyTcpServer::~EasyTcpServer()
{
	Close();
}

SOCKET EasyTcpServer::InitSocket()
{
	CELLNetwork::Init();

	if (_sock != INVALID_SOCKET)
	{
		CELLLog_Info("close old connection. ");
		Close();
	}

	_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sock == INVALID_SOCKET)
	{
		CELLLog_Error("create socket failed.");
	}
	else
	{
		CELLNetwork::Make_ReuseAddr(_sock);
		CELLLog_Info("create socket succeed.");
	}

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
		CELLLog_Info("[ERROR]bind[%d] socket failed.", port);
	else
		CELLLog_Info("bind socket succeed.");
	return ret;
}

int EasyTcpServer::Listen(int n)
{
	int ret = listen(_sock, n);
	if (ret == SOCKET_ERROR)
		CELLLog_Info("[ERROR]listen[%d] socket failed.", (int)_sock);
	else
		CELLLog_Info("listen socket[%d] succeed.", (int)_sock);
	return ret;
}

void EasyTcpServer::AddClientToCELLServer(CELLClientPtr& pclient)
{
	//_clients.push_back(pclient);

	auto pMinServer = _cellServers[0];
	for (auto pCELLServer : _cellServers)
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
		CELLLog_Error("accept socket failed.");
	else
	{
		if (_clientCount < _nMaxClient)
		{
			std::shared_ptr<CELLClient> ptr(new CELLClient(client));
			AddClientToCELLServer(ptr);
		}
		else
		{
#ifdef  _WIN32
			closesocket(client);
#else
			close(client);
#endif //  _WIN32

			CELLLog_Warring("Accept too much client[%d/%d]", _clientCount.load(), _nMaxClient - 1);
		}
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
				_cellServers.push_back(ser);
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
			_cellServers.clear();
		});
}

void EasyTcpServer::OnRun(CELLThread* pThread)
{
	while (pThread->isRun())
	{
		time4msg();

		//FD_ZERO(&fdRead);
		//FD_SET(_sock, &fdRead);
		fdRead.Zero();
		fdRead.Set(_sock);

		timeval t = { 0, 0 };
		int ret = select(_sock + 1, fdRead.Get(), nullptr, nullptr, &t);
		if (ret < 0)
		{
			CELLLog_Info("select task end. ");
			pThread->Exit();
			break;
		}

		if (fdRead.IsSet(_sock))
		{
			//FD_CLR(_sock, &fdRead);
			//fdRead.Clear();
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
		CELLLog_Info("thread<%d>,time<%f>,socket<%d>,client<%d>,recv<%d>,msg<%d>", (int)_cellServers.size(), t1, (int)_sock, _clientCount.load(), (int)(_recvCount / t1), _msgCount.load());
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
	CELLLog_Info("EasyTcpServer::Close() 1");
	_thread.Close();

	if (_sock == INVALID_SOCKET) return;

#ifdef _WIN32
	closesocket(_sock);
#else
	close(_sock);
#endif

	_sock = INVALID_SOCKET;

	CELLLog_Info("EasyTcpServer::Close() 2");
}
