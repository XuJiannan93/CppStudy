#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <Windows.h>
#include <WinSock2.h>

struct DataPackage
{
	int age;
	char name[32];
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
		int nLen = recv(_client, _recvbuf, 128, 0);
		if (nLen <= 0)
		{
			printf("client broken.");
			break;
		}

		printf("recv cmd : %s \n", _recvbuf);
		if (strcmp(_recvbuf, "getInfo") == 0)
		{
			DataPackage dp = { 80,"XiaoQiang" };
			send(_client, (const char*)&dp, sizeof(DataPackage), 0);
		}
		else
		{
			char msgBuf[] = "???";
			send(_client, msgBuf, strlen(msgBuf), 0);
		}
	}

	closesocket(_sock);

	WSACleanup();

	printf("server exit.\n");

	getchar();

	return 0;
}