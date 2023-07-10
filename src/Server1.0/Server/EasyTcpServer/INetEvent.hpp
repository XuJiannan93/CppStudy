#ifndef  _I_NET_EVENT_HPP_
#define _I_NET_EVENT_HPP_

#include "CellClient.h"

class CellServer;

class INetEvent
{
public:
	virtual void OnNetJoin(ClientSocketPtr& pClient) = 0;
	virtual void OnNetLeave(ClientSocketPtr& pClient) = 0;
	virtual void OnNetMsg(CellServer* pCellServer, ClientSocketPtr& pClient, netmsg_DataHeader* header) = 0;
	virtual void OnNetRecv(ClientSocketPtr& pClient) = 0;

private:

};


#endif // ! _I_NET_EVENT_HPP_
