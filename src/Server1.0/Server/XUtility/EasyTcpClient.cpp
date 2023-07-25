#include "EasyTcpClient.h"
#include "CELLNetwork.h"

EasyTcpClient::EasyTcpClient()
{
	//FD_ZERO(&_fdRead);
	//FD_ZERO(&_fdWrite);
	_fdRead.Zero();
	_fdWrite.Zero();

	_isConnected = false;
}

EasyTcpClient::~EasyTcpClient()
{
	Close();
}

SOCKET EasyTcpClient::InitSocket(int sendSize, int recvSize)
{
	CELLNetwork::Init();

	if (_pClient.get())
	{
		CELLLog_Info("close old connection. ");
		Close();
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET)
		CELLLog_Error("create socket failed.");
	else
	{
		CELLNetwork::Make_ReuseAddr(sock);
		_pClient = std::shared_ptr<CELLClient>(new CELLClient(sock, sendSize, recvSize));
	}

	return sock;
}

int EasyTcpClient::Connect(const char* ip, unsigned short port)
{
	if (_pClient == NULL)
	{
		if (InitSocket() == INVALID_SOCKET)
			return SOCKET_ERROR;
	}

	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(port);

#ifdef _WIN32
	_sin.sin_addr.S_un.S_addr = inet_addr(ip);
#else
	_sin.sin_addr.s_addr = inet_addr(ip);
#endif
	int ret = connect(_pClient->sockfd(), (sockaddr*)&_sin, sizeof(sockaddr_in));
	if (ret == SOCKET_ERROR)
	{
		_isConnected = false;
		CELLLog_Error("connect socket failed.");
	}
	else
	{
		_isConnected = true;
	}

	return ret;
}

void EasyTcpClient::Close()
{
	CELLLog_Info("sock closed!");
	_isConnected = false;

	if (_pClient.get() == nullptr) return;
	_pClient = NULL;
}

bool EasyTcpClient::OnRun(int microseconds)
{
	if (IsRun() == false)
		return false;

	SOCKET _sock = _pClient->sockfd();

	//FD_ZERO(&_fdRead);
	//FD_ZERO(&_fdWrite);
	//FD_SET(_sock, &_fdRead);

	_fdRead.Zero();
	_fdWrite.Zero();
	_fdRead.Set(_sock);

	timeval t = { 0, microseconds };
	int ret = 0;
	if (_pClient->NeedWrite())
	{
		//FD_SET(_sock, &_fdWrite);
		_fdWrite.Set(_sock);
		int ret = select(_sock + 1, _fdRead.Get(), _fdWrite.Get(), nullptr, &t);
	}
	else
	{
		int ret = select(_sock + 1, _fdRead.Get(), nullptr, nullptr, &t);
	}

	if (ret < 0)
	{
		CELLLog_Error("selcet[SELECT] task end...");
		Close();
		return false;
	}

	//if (FD_ISSET(_sock, &_fdRead))
	if (_fdRead.IsSet(_sock))
	{
		if (RecvData() == SOCKET_ERROR)
		{
			CELLLog_Error("selcet[READ] task end...");
			Close();
			return false;
		}
	}

	//if (FD_ISSET(_sock, &_fdWrite))
	if (_fdWrite.IsSet(_sock))
	{
		if (_pClient->SendDataImmediately() == SOCKET_ERROR)
		{
			CELLLog_Error("selcet[WRITE] task end...");
			Close();
			return false;
		}
	}

	return true;
}

int EasyTcpClient::SendData(const char* data, int len)
{
	if (IsRun() == false)
		return SOCKET_ERROR;

	return _pClient->SendData(data, len);
}

int EasyTcpClient::SendData(const DataHeaderPtr& header)
{
	if (IsRun() == false)
		return SOCKET_ERROR;

	return _pClient->SendData(header);
}


int EasyTcpClient::RecvData()
{
	if (IsRun() == false)
		return SOCKET_ERROR;

	int len = _pClient->RecvData();

	if (len > 0)
	{
		while (_pClient->HasMsg())
		{
			OnNetMsg(_pClient->front_msg());
			_pClient->pop_front_msg();
		}
	}

	return len;
}

