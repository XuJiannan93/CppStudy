#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <Windows.h>
#include <WinSock2.h>

enum CMD
{
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_NEW_USER_JOIN,
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
		cmd = CMD_LOGOUT_RESULT;
		result = 0;
	}
	int result;
};

struct NewUserJoin : public DataHeader
{
	NewUserJoin()
	{
		len = sizeof(NewUserJoin);
		cmd = CMD_NEW_USER_JOIN;
		sock = 0;
	}

	int sock;
};

int processor(SOCKET _sock)
{
	char szRecv[1024] = {};

	int nLen = recv(_sock, szRecv, sizeof(DataHeader), 0);
	DataHeader* header = (DataHeader*)szRecv;
	if (nLen <= 0)
	{
		printf("remote[%d] broken.\n", (int)_sock);
		return -1;
	}

	switch (header->cmd)
	{

	case CMD_LOGIN_RESULT:
	{
		recv(_sock, szRecv + sizeof(DataHeader), header->len - sizeof(DataHeader), 0);
		LoginResult* rst = (LoginResult*)(szRecv);
		printf("recv<%d> cmd len :login [%d][%d] \n", (int)_sock, rst->result, header->len);
	}
	break;

	case CMD_LOGOUT_RESULT:
	{
		recv(_sock, szRecv + sizeof(DataHeader), header->len - sizeof(DataHeader), 0);
		LogoutResult* rst = (LogoutResult*)(szRecv);
		printf("recv<%d> cmd len :logout [%d][%d] \n", (int)_sock, rst->result, header->len);

	}
	break;

	case CMD_NEW_USER_JOIN:
	{
		recv(_sock, szRecv + sizeof(DataHeader), header->len - sizeof(DataHeader), 0);
		NewUserJoin* join = (NewUserJoin*)(szRecv);
		printf("recv<%d> cmd len :new user join[%d][%d] \n", (int)_sock, (int)join->sock, header->len);
	}
	break;

	default:
	{
		header->cmd = CMD_ERROR;
		header->len = 0;

		send(_sock, (char*)&header, sizeof(DataHeader), 0);
	}
	break;
	}
}

int main()
{
	WORD ver = MAKEWORD(2, 2);
	WSADATA data;
	WSAStartup(ver, &data);

	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock == INVALID_SOCKET)
		printf("[ERROR]create socket failed.\n");
	else
		printf("create socket succeed.\n");

	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in)) == SOCKET_ERROR)
		printf("[ERROR]connect socket failed.\n");
	else
		printf("connect socket succeed.\n");

	char cmdbuf[128] = {};
	while (true)
	{
		fd_set fdReads;
		FD_ZERO(&fdReads);
		FD_SET(_sock, &fdReads);

		timeval t = { 1,0 };
		int ret = select(_sock, &fdReads, NULL, NULL, &t);
		if (ret < 0)
		{
			printf("selcet task end...\n");
			break;
		}

		if (FD_ISSET(_sock, &fdReads))
		{
			FD_CLR(_sock, &fdReads);
			if (processor(_sock) == -1)
			{
				printf("selcet task end...\n");
				break;
			}
		}

		printf("do some else...\n");

		Login login;
		strcpy(login.username, "xjn");
		strcpy(login.password, "123456");
		send(_sock, (char*)&login, sizeof(Login), 0);
		Sleep(1000);
		/*	scanf("%s", cmdbuf);
			if (strcmp(cmdbuf, "exit") == 0)
				break;
			else if (strcmp(cmdbuf, "login") == 0)
			{
				Login login;
				strcpy(login.username, "xjn");
				strcpy(login.password, "123456");
				send(_sock, (const char*)&login, sizeof(Login), 0);

				LoginResult rst = {};
				recv(_sock, (char*)&rst, sizeof(LoginResult), 0);

				printf("LoginResult %d \n", rst.result);
			}
			else if (strcmp(cmdbuf, "logout") == 0)
			{
				Logout logout;
				strcpy(logout.username, "xjn");
				send(_sock, (const char*)&logout, sizeof(Logout), 0);

				LogoutResult rst = {};
				recv(_sock, (char*)&rst, sizeof(LogoutResult), 0);

				printf("LogoutResult %d \n", rst.result);
			}
			else
			{
				printf("???\n");
			}*/
	}

	closesocket(_sock);

	WSACleanup();

	printf("client exit.\n");
	getchar();

	return 0;
}