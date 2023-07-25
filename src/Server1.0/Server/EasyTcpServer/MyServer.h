#ifndef _MY_SERVER_H_
#define _MY_SERVER_H_

#include "MessageHeader.hpp"
#include "EasyTcpServer.h"

class MyServer : public EasyTcpServer
{
public:
	MyServer();

	void OnNetJoin(CELLClientPtr& pClient);
	void OnNetLeave(CELLClientPtr& pClient);
	void OnNetMsg(CELLServer* pCELLServer, CELLClientPtr& pClient, netmsg_DataHeader* header);

private:
	bool _bSendBack = false;
	bool _bSendFull = false;
	bool _bCheckMsgID = false;
};
#endif