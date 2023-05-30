#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <Windows.h>
#include <WinSock2.h>

enum CMD
{
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESLUT,
	CMD_ERROR,
};

struct DataHeader
{
	short len;
	short cmd;
};

struct Login : public DataHeader
{
	Login()
	{
		len = sizeof(Login);
		cmd = CMD_LOGIN;
	}

	char username[32];
	char password[32];
};

struct LoginResult : public DataHeader
{
	LoginResult()
	{
		len = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;
	}
	int result;
};

struct Logout : public DataHeader
{
	Logout()
	{
		len = sizeof(Logout);
		cmd = CMD_LOGOUT;
	}
	char username[32];
};

struct LogoutResult : public DataHeader
{
	LogoutResult()
	{
		len = sizeof(LogoutResult);
		cmd = CMD_LOGOUT_RESLUT;
		result = 0;
	}
	int result;
};

int main()
{
	WORD ver = MAKEWORD(2, 2);
	WSADATA data;
	WSAStartup(ver, &data);

	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);	//host to net unsigned short
	//_sin.sin_addr.S_un.S_addr = INADDR_ANY;		//inet_addr("127.0.0.1");
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (bind(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in)) == SOCKET_ERROR)
		printf("[ERROR]bind socket failed.\n");
	else
		printf("bind socket succeed.\n");

	if (listen(_sock, 5) == SOCKET_ERROR)
		printf("[ERROR]listen socket failed.\n");
	else
		printf("listen socket succeed.\n");

	sockaddr_in clientAddr = {};
	int nAddrLen = sizeof(clientAddr);
	SOCKET _client = INVALID_SOCKET;
	char msgBuf[] = "HELLO, I AM SERVER. \n";

	_client = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
	if (_client == INVALID_SOCKET)
		printf("[ERROR]accept socket failed.\n");

	printf("new client[%d][IP : %s] linked in.\n", (int)_client, inet_ntoa(clientAddr.sin_addr));
	char _recvbuf[128] = {};

	while (true)
	{
		DataHeader header = {};

		int nLen = recv(_client, (char*)&header, sizeof(DataHeader), 0);
		if (nLen <= 0)
		{
			printf("client broken.");
			break;
		}

		//printf("recv cmd len :%d[%d] \n", header.cmd, header.len);

		switch (header.cmd)
		{

		case CMD_LOGIN:
		{
			Login login = {};
			recv(_client, (char*)&login + sizeof(DataHeader), sizeof(Login) - sizeof(DataHeader), 0);
			printf("recv cmd len :login [%s][%s][%d] \n", login.username, login.password, login.len);

			LoginResult rst;
			send(_client, (char*)&rst, sizeof(LoginResult), 0);
		}
		break;

		case CMD_LOGOUT:
		{
			Logout logout = {};
			recv(_client, (char*)&logout + sizeof(DataHeader), sizeof(Logout) - sizeof(DataHeader), 0);
			printf("recv cmd len :logout [%s][%d] \n", logout.username, header.len);

			LogoutResult rst;
			send(_client, (char*)&rst, sizeof(LogoutResult), 0);
		}
		break;

		default:
		{
			header.cmd = CMD_ERROR;
			header.len = 0;

			send(_client, (char*)&header, sizeof(DataHeader), 0);
		}
		break;
		}
	}

	closesocket(_sock);

	WSACleanup();

	printf("server exit.\n");

	getchar();

	return 0;
}