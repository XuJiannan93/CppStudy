#ifndef  _I_NET_EVENT_HPP_
#define _I_NET_EVENT_HPP_

#include "CELLClient.h"

class CELLServer;

class INetEvent
{
public:
	virtual void OnNetJoin(CELLClientPtr& pClient) = 0;
	virtual void OnNetLeave(CELLClientPtr& pClient) = 0;
	virtual void OnNetMsg(CELLServer* pCELLServer, CELLClientPtr& pClient, netmsg_DataHeader* header) = 0;
	virtual void OnNetRecv(CELLClientPtr& pClient) = 0;

private:

};


#endif // ! _I_NET_EVENT_HPP_
