#ifndef EASY_TCP_CLIENT_HPP
#define EASY_TCP_CLIENT_HPP

#include <stdio.h>
#include <thread>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#define SOCKET int
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR           (-1)

#endif

#include "MessageHeader.hpp"
#include "CELLClient.h"

#ifndef  RECV_BUFF_SIZE
#define RECV_BUFF_SIZE 10240
#endif // ! RECV_BUFF_SIZE

class EasyTcpClient
{

public:
	EasyTcpClient();
	virtual ~EasyTcpClient();

	void InitSocket();
	int Connect(const char* ip, unsigned short port);
	void Close();
	bool OnRun();
	bool IsRun() { return _isConnected; }
	int SendData(const DataHeaderPtr& header);
	int RecvData();
	virtual void OnNetMsg(netmsg_DataHeader* header);

private:

private:
	fd_set _fdRead;
	fd_set _fdWrite;

	CELLClientPtr _pClient;
	char _szRecv[RECV_BUFF_SIZE] = {};
	bool _isConnected;
};

#endif