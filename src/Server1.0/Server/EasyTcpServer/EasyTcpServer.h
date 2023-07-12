#ifndef  _EASY_TCP_SERVER_H_
#define __EASY_TCP_SERVER_H__

#include <stdio.h>
#include <vector>
#include <thread>
#include <mutex>

#include "CELL.hpp"
#include "CELLTimestamp.h"
#include "CELLClient.h"
#include "CELLServer.h"
#include "INetEvent.hpp"
#include "CELLThread.h"

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
	void AddClientToCELLServer(CELLClientPtr& pclient);
	void Start(int nCELLServer);
	void SendDataToAll(netmsg_DataHeader* header);
	void Close();
	virtual void OnNetJoin(CELLClientPtr& pClient);
	virtual void OnNetLeave(CELLClientPtr& pClient);
	virtual void OnNetMsg(CELLServer* pCELLServer, CELLClientPtr& pClient, netmsg_DataHeader* header);
	virtual void OnNetRecv(CELLClientPtr& pClient);

private:
	void OnRun(CELLThread* pThread);
	virtual void time4msg(/*SOCKET client, DataHeader* header*/);

private:
	SOCKET _sock;
	char _szRecv[RECV_BUFF_SIZE] = {};
	//std::vector<ClientSocketPtr> _clients;
	std::vector<CELLServerPtr> _CELLServers;
	CELLTimestamp _tTime;
	std::mutex _mutex;
	CELLThread _thread;

protected:
	std::atomic_int _msgCount;
	std::atomic_int _recvCount;
	std::atomic_int _clientCount;

};

#endif //  _EASY_TCP_SERVER_H_
