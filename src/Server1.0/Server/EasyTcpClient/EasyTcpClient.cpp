#include "EasyTcpClient.h"
#include "CELLNetwork.h"

EasyTcpClient::EasyTcpClient()
{
	FD_ZERO(&_fdRead);
	FD_ZERO(&_fdWrite);

	_isConnected = false;
}

EasyTcpClient::~EasyTcpClient()
{
	Close();
}

void EasyTcpClient::InitSocket()
{
	CELLNetwork::Init();

	if (_pClient.get())
	{
		printf("close old connection. \n");
		Close();
	}

	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (_sock == INVALID_SOCKET)
		printf("[ERROR]create socket failed.\n");
	else
	{
		_pClient = std::shared_ptr<CELLClient>(new CELLClient(_sock));
	}
}

int EasyTcpClient::Connect(const char* ip, unsigned short port)
{
	if (_pClient == NULL || _pClient.get() == nullptr)
		InitSocket();

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
		printf("[ERROR]connect socket failed.\n");
	}
	else
	{
		_isConnected = true;
	}

	return ret;
}

void EasyTcpClient::Close()
{
	printf("sock closed!\n");
	_isConnected = false;

	if (_pClient.get() == nullptr) return;
	_pClient = NULL;
}

bool EasyTcpClient::OnRun()
{
	if (IsRun() == false)
		return false;

	SOCKET _sock = _pClient->sockfd();

	FD_ZERO(&_fdRead);
	FD_SET(_sock, &_fdRead);

	FD_ZERO(&_fdWrite);

	timeval t = { 0, 1 };
	int ret = 0;
	if (_pClient->NeedWrite())
	{
		FD_SET(_sock, &_fdWrite);
		int ret = select(_sock + 1, &_fdRead, &_fdWrite, nullptr, &t);
	}
	else
	{
		int ret = select(_sock + 1, &_fdRead, nullptr, nullptr, &t);
	}

	if (ret < 0)
	{
		printf("selcet task end...\n");
		return false;
	}

	if (FD_ISSET(_sock, &_fdRead))
	{
		if (RecvData() == SOCKET_ERROR)
		{
			printf("[READ ERROR]selcet task end...\n");
			Close();
			return false;
		}
	}

	if (FD_ISSET(_sock, &_fdWrite))
	{
		if (_pClient->SendDataImmediately() == SOCKET_ERROR)
		{
			printf("[WRITE ERROR]selcet task end...\n");
			Close();
			return false;
		}
	}

	return true;
}

int EasyTcpClient::SendData(const DataHeaderPtr& header)
{
	if (IsRun() == false)
		return SOCKET_ERROR;

	return _pClient->SendData(header);
}


int EasyTcpClient::RecvData()
{
	int len = _pClient->RecvData();

	if (len > 0)
	{
		while (_pClient->HasMsg())
		{
			OnNetMsg(_pClient->front_msg());
			_pClient->pop_front_msg();
		}
	}

	return 0;
}

void EasyTcpClient::OnNetMsg(netmsg_DataHeader* header)
{
	switch (header->cmd)
	{

	case CMD_LOGIN_RESULT:
	{
		netmsg_LoginResult* rst = (netmsg_LoginResult*)header;
		//printf("recv<%d> cmd[LOGIN][%d] len[%d] \n", (int)_sock, rst->result, header->len);
	}
	break;

	case CMD_LOGOUT_RESULT:
	{
		netmsg_LogoutResult* rst = (netmsg_LogoutResult*)header;
		//printf("recv<%d> cmd[LOGOUT][%d] len[%d] \n", (int)_sock, rst->result, header->len);
	}
	break;

	case CMD_NEW_USER_JOIN:
	{
		netmsg_NewUserJoin* join = (netmsg_NewUserJoin*)header;
		//printf("recv<%d> cmd[NEW_USER_JOIN][%d] len[%d] \n", (int)_sock, (int)join->sock, header->len);
	}
	break;

	case CMD_S2C_HEART_BEAT:
		break;

	case CMD_ERROR:
		printf("recv undefine message! \n");

	default:
	{
		/*	header->cmd = CMD_ERROR;
			header->len = 0;
			send(_sock, (const char*)header, sizeof(DataHeader), 0);*/

		printf("send error.\n");
	}
	break;
	}
}
