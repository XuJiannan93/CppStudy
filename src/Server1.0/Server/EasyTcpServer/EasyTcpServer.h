#ifndef  EASY_TCP_SERVER_H
#define EASY_TCP_SERVER_H

#include <stdio.h>
#include <vector>

#ifdef _WIN32
#define FD_SETSIZE 1024
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>

#ifndef socklen_t
#define socklen_t int
#endif // !socklen_t

#else
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define SOCKET int
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR (-1)
#endif

#include "MessageHeader.hpp"
#include "ClientSocket.h"

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
	int SendData(SOCKET client, DataHeader* header);
	void SendDataToAll(DataHeader* header);
	int RecvData(ClientSocket* client);
	virtual void OnNetMsg(SOCKET client, DataHeader* header);
	void Close();

private:
	SOCKET _sock;
	char _szRecv[RECV_BUFF_SIZE] = {};
	std::vector<ClientSocket*> _clients;
};

#endif //  EASY_TCP_SERVER_H
