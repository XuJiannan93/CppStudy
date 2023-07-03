#ifndef  INETEVENT_HPP
#define INETEVENT_HPP

#include "ClientSocket.h"

class INetEvent
{
public:
	virtual void OnNetJoin(ClientSocket* pClient) = 0;
	virtual void OnNetLeave(ClientSocket* pClient) = 0;
	virtual void OnNetMsg(ClientSocket* pClient, DataHeader* header) = 0;
	virtual void OnNetRecv(ClientSocket* pClient) = 0;

private:

};


#endif // ! INETEVENT_HPP
