#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include <stdio.h>
#include <vector>

#ifdef _WIN32
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

#ifndef  RECV_BUFF_SIZE
#define RECV_BUFF_SIZE 10240
#endif // ! RECV_BUFF_SIZE

#include "MessageHeader.hpp"


class ClientSocket
{
private:
	SOCKET _sockfd;
	char _szMsgBuf[RECV_BUFF_SIZE * 5] = {};
	int _lastPos = 0;

public:
	SOCKET sockfd();
	char* msgBuf();
	int getLastPos();
	void setLastPos(int pos);
	int SendData(DataHeader* header);

	ClientSocket(SOCKET sockfd = INVALID_SOCKET);
};

#endif // !CLIENT_SOCKET_H
