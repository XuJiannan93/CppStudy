#ifndef  EASY_TCP_SERVER_H
#define EASY_TCP_SERVER_H

#include <stdio.h>
#include <vector>
#include <thread>
#include <mutex>

#ifdef _WIN32
#define FD_SETSIZE 2506 
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

//#include "MessageHeader.hpp"
#include "CELLTimestamp.h"
#include "ClientSocket.h"
#include "CellServer.h"
#include "INetEvent.hpp"

//#define _CELL_SERVER_THREAD_COUNT 4

class EasyTcpServer : public INetEvent
{
public:
	EasyTcpServer();
	virtual ~EasyTcpServer();

	SOCKET InitSocket();
	int Bind(const char* ip, unsigned short port);
	int Listen(int n);
	SOCKET Accept();
	void AddClientToCellServer(ClientSocket* pclient);
	void Start(int nCellServer);
	bool IsRun() { return _sock != INVALID_SOCKET; }
	bool OnRun();
	void SendDataToAll(DataHeader* header);
	int RecvData(ClientSocket* client);
	void Close();
	virtual void OnNetJoin(ClientSocket* pClient);
	virtual void OnNetLeave(ClientSocket* pClient);
	virtual void OnNetMsg(ClientSocket* pClient, DataHeader* header);
	virtual void OnNetRecv(ClientSocket* pClient);

private:
	virtual void time4msg(/*SOCKET client, DataHeader* header*/);

private:
	SOCKET _sock;
	char _szRecv[RECV_BUFF_SIZE] = {};
	//std::vector<ClientSocket*> _clients;
	std::vector<CellServer*> _cellServers;
	CELLTimestamp _tTime;
	std::mutex _mutex;

protected:
	std::atomic_int _msgCount;
	std::atomic_int _recvCount;
	std::atomic_int _clientCount;

};

#endif //  EASY_TCP_SERVER_H
