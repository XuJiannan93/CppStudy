#ifndef CELL_SERVER_H
#define CELL_SERVER_H

#include <stdio.h>
#include <vector>
#include <mutex>
#include <functional>
#include <atomic>
#include <map>

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
#include "INetEvent.hpp"

class CellServer
{
public:
	CellServer(SOCKET sock = INVALID_SOCKET);
	virtual ~CellServer();
	void Start();
	bool IsRun() { return _sock != INVALID_SOCKET; }
	void OnRun();
	//int SendData(SOCKET client, DataHeader* header);
	int RecvData(ClientSocket* client);
	virtual void OnNetMsg(ClientSocket* pClient, DataHeader* header);
	void Close();
	size_t GetClientCount() { return _clients.size() + _clientsBuff.size(); }
	void addClient(ClientSocket* pClient);
	void SetEventObj(INetEvent* evt);

private:
	SOCKET _sock;

	std::map<SOCKET, ClientSocket*> _clients;
	std::vector<ClientSocket*> _clientsBuff;

	//char _szRecv[RECV_BUFF_SIZE] = {};

	std::mutex _mutex;
	std::thread _thread;
	INetEvent* _pNetEvent;

	fd_set _fdRead_bak;
	bool _clients_changed = true;
	SOCKET _maxSock;

};

#endif