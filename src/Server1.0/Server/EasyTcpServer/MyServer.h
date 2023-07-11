#ifndef _MY_SERVER_H_
#define _MY_SERVER_H_

#include "MessageHeader.hpp"
#include "EasyTcpServer.h"

class MyServer : public EasyTcpServer
{
public:
	void OnNetJoin(ClientSocketPtr& pClient);

	void OnNetLeave(ClientSocketPtr& pClient);

	void OnNetMsg(CELLServer* pCELLServer, ClientSocketPtr& pClient, netmsg_DataHeader* header);


};
#endif