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
		scanf("%s", cmdbuf);
		if (strcmp(cmdbuf, "exit") == 0)
			break;
		else
			send(_sock, cmdbuf, strlen(cmdbuf) + 1, 0);

		char recvBuf[256] = {};
		int nlen = recv(_sock, recvBuf, 256, 0);
		if (nlen > 0)
		{
			DataPackage* dp = (DataPackage*)recvBuf;
			printf("recv: age[%d] name[%s] \n", dp->age, dp->name);
		}
		else
			printf("recv error!\n");
	}

	closesocket(_sock);

	WSACleanup();

	printf("client exit.\n");
	getchar();

	return 0;
}