#ifndef  EASY_TCP_SERVER_H
#define EASY_TCP_SERVER_H

#include <stdio.h>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>

#define socklen_t int

#else
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define SOCKET int
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR (-1)
#endif

#include "MessageHeader.hpp"

class EasyTcpServer
{
public:
	EasyTcpServer();
	virtual ~EasyTcpServer();
	SOCKET InitSocket();
	int Bind(const char* ip, unsigned short port);
	int Listen(int n);
	SOCKET Accept();
	bool IsRun() { return _sock != INVALID_SOCKET; }
	bool OnRun();
	int SendData(SOCKET _client, DataHeader* header);
	void SendDataToAll(DataHeader* header);
	int RecvData(SOCKET _client);
	virtual void OnNetMsg(SOCKET _client, DataHeader* header);
	void Close();

private:
	SOCKET _sock;
	std::vector<SOCKET> g_clients;
};

#endif //  EASY_TCP_SERVER_H
