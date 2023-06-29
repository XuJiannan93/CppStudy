#ifndef  INETEVENT_HPP
#define INETEVENT_HPP

#include "ClientSocket.h"

class INetEvent
{
public:
	virtual void OnLeave(ClientSocket* pClient) = 0;
	virtual void OnNetMsg(SOCKET cSock, DataHeader* header) = 0;

private:

};


#endif // ! INETEVENT_HPP
