#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <Windows.h>
#include <WinSock2.h>
#include <vector>

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

std::vector<SOCKET> g_clients;

int processor(SOCKET _client)
{
	char szRecv[1024] = {};

	int nLen = recv(_client, szRecv, sizeof(DataHeader), 0);
	DataHeader* header = (DataHeader*)szRecv;
	if (nLen <= 0)
	{
		printf("client[%d] broken.\n", (int)_client);
		return -1;
	}

	switch (header->cmd)
	{

	case CMD_LOGIN:
	{
		recv(_client, szRecv + sizeof(DataHeader), header->len - sizeof(DataHeader), 0);
		Login* login = (Login*)(szRecv);
		printf("recv<%d> cmd len :login [%s][%s][%d] \n", (int)_client, login->username, login->password, header->len);

		LoginResult rst;
		send(_client, (char*)&rst, sizeof(LoginResult), 0);
	}
	break;

	case CMD_LOGOUT:
	{
		recv(_client, szRecv + sizeof(DataHeader), header->len - sizeof(DataHeader), 0);
		Logout* logout = (Logout*)(szRecv);
		printf("recv<%d> cmd len :logout [%s][%d] \n", (int)_client, logout->username, header->len);

		LogoutResult rst;
		send(_client, (char*)&rst, sizeof(LogoutResult), 0);
	}
	break;

	default:
	{
		header->cmd = CMD_ERROR;
		header->len = 0;

		send(_client, (char*)&header, sizeof(DataHeader), 0);
	}
	break;
	}
}

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

	char _recvbuf[128] = {};

	while (true)
	{
		fd_set fdRead;
		fd_set fdWrite;
		fd_set fdExc;

		FD_ZERO(&fdRead);
		FD_ZERO(&fdWrite);
		FD_ZERO(&fdExc);

		FD_SET(_sock, &fdRead);
		FD_SET(_sock, &fdWrite);
		FD_SET(_sock, &fdExc);

		for (int n = (int)g_clients.size() - 1; n >= 0; n--)
		{
			FD_SET(g_clients[n], &fdRead);
		}

		timeval t = { 1,0 };
		int ret = select(_sock + 1, &fdRead, &fdWrite, &fdExc, &t);
		if (ret < 0)
		{
			printf("select task end. \n");
			break;
		}

		if (FD_ISSET(_sock, &fdRead))
		{
			FD_CLR(_sock, &fdRead);

			sockaddr_in clientAddr = {};
			int nAddrLen = sizeof(clientAddr);
			SOCKET _client = INVALID_SOCKET;

			_client = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
			if (_client == INVALID_SOCKET)
				printf("[ERROR]accept socket failed.\n");

			for (int n = (int)g_clients.size() - 1; n >= 0; n--)
			{
				NewUserJoin join = {};
				join.sock = (int)_client;
				send(g_clients[n], (const char*)&join, sizeof(NewUserJoin), 0);
			}

			g_clients.push_back(_client);
			printf("new client[%d][IP : %s] linked in.\n", (int)_client, inet_ntoa(clientAddr.sin_addr));
		}

		for (int n = 0; n < fdRead.fd_count; n++)
		{
			if (processor(fdRead.fd_array[n]) == -1)
			{
				auto iter = find(g_clients.begin(), g_clients.end(), fdRead.fd_array[n]);
				if (iter != g_clients.end())
				{
					g_clients.erase(iter);
				}
			}
		}

		printf("do something else...\n");
	}

	for (int n = (int)g_clients.size() - 1; n >= 0; n--)
	{
		closesocket(g_clients[n]);
	}

	closesocket(_sock);

	WSACleanup();

	printf("server exit.\n");

	getchar();

	return 0;
}