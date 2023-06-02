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

class EasyTcpClient
{
private:
	SOCKET _sock;

public:
	EasyTcpClient();
	virtual ~EasyTcpClient();

	void InitSocket();
	int Connect(const char* ip, unsigned short port);
	void Close();
	bool OnRun();
	bool IsRun() { return _sock != INVALID_SOCKET; }
	int SendData(DataHeader* header);
	int RecvData();
	virtual void OnNetMsg(DataHeader* header);
};

#endif