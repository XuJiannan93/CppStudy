#include "EasyTcpClient.h"

EasyTcpClient::EasyTcpClient()
{
	_sock = INVALID_SOCKET;
	_isConnected = false;
}

EasyTcpClient::~EasyTcpClient()
{
	Close();
}

void EasyTcpClient::InitSocket()
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
		printf("[ERROR]create socket failed.\n");
	else
	{
		//printf("create socket succeed.\n");
	}
}

int EasyTcpClient::Connect(const char* ip, unsigned short port)
{
	if (_sock == INVALID_SOCKET)
		InitSocket();

	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(port);

#ifdef _WIN32
	_sin.sin_addr.S_un.S_addr = inet_addr(ip);
#else
	_sin.sin_addr.s_addr = inet_addr(ip);
#endif
	int ret = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
	if (ret == SOCKET_ERROR)
	{
		_isConnected = false;
		printf("[ERROR]connect socket failed.\n");
	}
	else
	{
		_isConnected = true;
		//printf("connect socket succeed.\n");
	}

	return ret;
}

void EasyTcpClient::Close()
{
	if (_sock == INVALID_SOCKET) return;

#ifdef _WIN32
	closesocket(_sock);
	WSACleanup();
#else
	close(_sock);
#endif

	_sock = INVALID_SOCKET;
	_isConnected = false;
}

bool EasyTcpClient::OnRun()
{
	if (IsRun() == false)
		return false;

	fd_set fdReads;
	FD_ZERO(&fdReads);
	FD_SET(_sock, &fdReads);

	timeval t = { 1,0 };
	//printf("select begin\n");
	int ret = select(_sock + 1, &fdReads, NULL, NULL, 0);
	//printf("select end\n");
	if (ret < 0)
	{
		printf("selcet task end...\n");
		return false;
	}

	if (FD_ISSET(_sock, &fdReads))
	{
		FD_CLR(_sock, &fdReads);
		if (RecvData() == -1)
		{
			printf("selcet task end...\n");
			return false;
		}
	}

	return true;
}

int EasyTcpClient::SendData(DataHeader* header, int nLen)
{
	int ret = SOCKET_ERROR;
	if (IsRun() && header)
	{
		ret = send(_sock, (const char*)header, nLen, 0);
		if (ret == SOCKET_ERROR)
			Close();
	}

	return SOCKET_ERROR;
}

int EasyTcpClient::RecvData()
{
	int nLen = (int)recv(_sock, _szRecv, RECV_BUFF_SIZE, 0);
	if (nLen <= 0)
	{
		printf("remote[%d] broken.\n", (int)_sock);
		return -1;
	}
	memcpy(_szMsgBuf + _lastPos, _szRecv, nLen);
	_lastPos += nLen;

	while (_lastPos >= sizeof(DataHeader))
	{
		DataHeader* header = (DataHeader*)_szMsgBuf;
		if (_lastPos < header->len)
			break;

		int pos = _lastPos - header->len;
		OnNetMsg(header);
		memcpy(_szMsgBuf, _szMsgBuf + header->len, _lastPos - header->len);
		_lastPos = pos;
	}

	return 0;
}

void EasyTcpClient::OnNetMsg(DataHeader* header)
{
	switch (header->cmd)
	{

	case CMD_LOGIN_RESULT:
	{
		LoginResult* rst = (LoginResult*)header;
		//printf("recv<%d> cmd[LOGIN][%d] len[%d] \n", (int)_sock, rst->result, header->len);
	}
	break;

	case CMD_LOGOUT_RESULT:
	{
		LogoutResult* rst = (LogoutResult*)header;
		//printf("recv<%d> cmd[LOGOUT][%d] len[%d] \n", (int)_sock, rst->result, header->len);
	}
	break;

	case CMD_NEW_USER_JOIN:
	{
		NewUserJoin* join = (NewUserJoin*)header;
		//printf("recv<%d> cmd[NEW_USER_JOIN][%d] len[%d] \n", (int)_sock, (int)join->sock, header->len);
	}
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
