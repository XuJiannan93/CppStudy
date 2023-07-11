#ifndef _CELL_SERVER_H_
#define _CELL_SERVER_H_

#include <stdio.h>
#include <vector>
#include <mutex>
#include <functional>
#include <atomic>
#include <map>
#include <memory>

#include "Cell.hpp"
#include "MessageHeader.hpp"
#include "CellClient.h"
#include "INetEvent.hpp"
#include "CellTaskServer.h"
#include "CELLSemaphore.h"

//typedef std::shared_ptr<CellSendMsg2ClientTask> CellSendMsg2ClientTaskPtr;

class CellServer
{
public:
	CellServer(int id);
	virtual ~CellServer();

	void Start();
	void Close();
	//bool IsRun() { return _sock != INVALID_SOCKET; }
	void OnRun(CELLThread* pThread);
	int RecvData(ClientSocketPtr client);
	virtual void OnNetMsg(ClientSocketPtr& pClient, netmsg_DataHeader* header);
	size_t GetClientCount() { return _clients.size() + _clientsBuff.size(); }
	void addClient(ClientSocketPtr& pClient);
	void SetEventObj(INetEvent* evt);
	void AddSendTask(ClientSocketPtr& pClient, DataHeaderPtr& header);

private:
	void ReadData(fd_set& fdRead);
	void CheckTime();

private:
	//SOCKET _sock;
	std::map<SOCKET, ClientSocketPtr> _clients;
	std::vector<ClientSocketPtr> _clientsBuff;

	std::mutex _mutex;
	INetEvent* _pNetEvent;
	fd_set _fdRead_bak;
	SOCKET _maxSock;
	CellTaskServer _taskServer;
	time_t _old_time;
	CELLThread _thread;
	int _id = -1;
	bool _clients_changed = true;

};

#ifndef _SHARED_PTR_CELL_SERVER_
#define _SHARED_PTR_CELL_SERVER_
typedef std::shared_ptr<CellServer> CellServerPtr;

#endif // !_SHARED_PTR_CELL_SERVER_

#endif