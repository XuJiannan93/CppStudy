#ifndef  EASY_TCP_SERVER_H
#define EASY_TCP_SERVER_H

#include <stdio.h>
#include <vector>
#include <thread>
#include <mutex>

#include "Cell.hpp"
#include "CELLTimestamp.h"
#include "CellClient.h"
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
	void AddClientToCellServer(ClientSocketPtr& pclient);
	void Start(int nCellServer);
	bool IsRun() { return _sock != INVALID_SOCKET; }
	bool OnRun();
	void SendDataToAll(netmsg_DataHeader* header);
	int RecvData(ClientSocketPtr& client);
	void Close();
	virtual void OnNetJoin(ClientSocketPtr& pClient);
	virtual void OnNetLeave(ClientSocketPtr& pClient);
	virtual void OnNetMsg(CellServer* pCellServer, ClientSocketPtr& pClient, netmsg_DataHeader* header);
	virtual void OnNetRecv(ClientSocketPtr& pClient);

private:
	virtual void time4msg(/*SOCKET client, DataHeader* header*/);

private:
	SOCKET _sock;
	char _szRecv[RECV_BUFF_SIZE] = {};
	//std::vector<ClientSocketPtr> _clients;
	std::vector<CellServerPtr> _cellServers;
	CELLTimestamp _tTime;
	std::mutex _mutex;

protected:
	std::atomic_int _msgCount;
	std::atomic_int _recvCount;
	std::atomic_int _clientCount;

};

#endif //  EASY_TCP_SERVER_H
