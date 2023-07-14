#ifndef _CELL_SERVER_H_
#define _CELL_SERVER_H_

#include <stdio.h>
#include <vector>
#include <mutex>
#include <functional>
#include <atomic>
#include <map>
#include <memory>

#include "CELL.hpp"
#include "MessageHeader.hpp"
#include "CELLClient.h"
#include "INetEvent.hpp"
#include "CELLTaskServer.h"
#include "CELLSemaphore.h"

class CELLServer
{
public:
	CELLServer(int id);
	virtual ~CELLServer();

	void Start();
	void Close();
	//bool IsRun() { return _sock != INVALID_SOCKET; }
	void OnRun(CELLThread* pThread);
	int RecvData(CELLClientPtr client);
	virtual void OnNetMsg(CELLClientPtr& pClient, netmsg_DataHeader* header);
	size_t GetClientCount() { return _clients.size() + _clientsBuff.size(); }
	void addClient(CELLClientPtr& pClient);
	void SetEventObj(INetEvent* evt);
	void AddSendTask(CELLClientPtr& pClient, DataHeaderPtr& header);

private:
	void _ReadData(fd_set& fdRead);
	void _WriteData(fd_set& fdWrite);
	void _CheckTime();
	void _OnClientLeave(CELLClientPtr pClient);

private:
	//SOCKET _sock;
	std::map<SOCKET, CELLClientPtr> _clients;
	std::vector<CELLClientPtr> _clientsBuff;

	std::mutex _mutex;
	INetEvent* _pNetEvent;

	fd_set _fdRead;
	fd_set _fdWrite;
	//fd_set _fdExc;
	fd_set _fdRead_bak;

	SOCKET _maxSock;
	CELLTaskServer _taskServer;
	time_t _old_time;
	CELLThread _thread;
	int _id = -1;
	bool _clients_changed = true;

};

#ifndef _SHARED_PTR_CELL_SERVER_
#define _SHARED_PTR_CELL_SERVER_
typedef std::shared_ptr<CELLServer> CELLServerPtr;

#endif // !_SHARED_PTR_CELL_SERVER_

#endif