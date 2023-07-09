#ifndef  INETEVENT_HPP
#define INETEVENT_HPP

#include "ClientSocket.h"

class CellServer;

class INetEvent
{
public:
	virtual void OnNetJoin(ClientSocketPtr& pClient) = 0;
	virtual void OnNetLeave(ClientSocketPtr& pClient) = 0;
	virtual void OnNetMsg(CellServer* pCellServer, ClientSocketPtr& pClient, DataHeader* header) = 0;
	virtual void OnNetRecv(ClientSocketPtr& pClient) = 0;

private:

};


#endif // ! INETEVENT_HPP
