#ifndef _CLIENT_SOCKET_H
#define _CLIENT_SOCKET_H

#include <stdio.h>
#include <vector>
#include <memory>

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
#define SEND_BUFF_SIZE RECV_BUFF_SIZE
#endif // ! RECV_BUFF_SIZE

#include "MessageHeader.hpp"
#include "ObjectPoolBase.hpp"

class ClientSocket : public ObjectPoolBase<ClientSocket, 10000>
{
private:
	SOCKET _sockfd;
	char _szMsgBuf[RECV_BUFF_SIZE] = {};
	char _szSendBuf[SEND_BUFF_SIZE] = {};
	int _lastPos = 0;
	int _lastSendPos = 0;

public:
	SOCKET sockfd();
	char* msgBuf();
	int getLastPos();
	void setLastPos(int pos);
	int SendData(DataHeaderPtr& header);

	ClientSocket(SOCKET sockfd = INVALID_SOCKET);
	~ClientSocket();
};

#ifndef _SHARED_PTR_CLIENT_SOCKET_
#define _SHARED_PTR_CLIENT_SOCKET_
typedef std::shared_ptr<ClientSocket> ClientSocketPtr;
#endif // !_SHARED_PTR_CLIENT_SOCKET_

#endif // !_CLIENT_SOCKET_H
