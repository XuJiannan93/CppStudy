#ifndef _MY_SERVER_H_
#define _MY_SERVER_H_

#include "MessageHeader.hpp"
#include "EasyTcpServer.h"

class MyServer : public EasyTcpServer
{
public:
	void OnNetJoin(CELLClientPtr& pClient);

	void OnNetLeave(CELLClientPtr& pClient);

	void OnNetMsg(CELLServer* pCELLServer, CELLClientPtr& pClient, netmsg_DataHeader* header);


};
#endif