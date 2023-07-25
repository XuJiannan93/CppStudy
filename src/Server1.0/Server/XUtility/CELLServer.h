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
#include "CELLFDSet.h"

class _X_UTILITY_API_ CELLServer
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
	void _ReadData();
	void _WriteData();
	void _CheckTime();
	void _OnClientLeave(CELLClientPtr pClient);
	bool _DoSelect();
	void _DoMsg();

private:
	//SOCKET _sock;
	std::map<SOCKET, CELLClientPtr> _clients;
	std::vector<CELLClientPtr> _clientsBuff;

	std::mutex _mutex;
	INetEvent* _pNetEvent;

	CELLFDSet _fdRead;
	CELLFDSet _fdWrite;
	//fd_set _fdExc;
	CELLFDSet _fdRead_bak;

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