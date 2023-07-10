#ifndef _MY_SERVER_H_
#define _MY_SERVER_H_

#include "MessageHeader.hpp"
#include "EasyTcpServer.h"
#include "CellS2CTask.h"


class MyServer : public EasyTcpServer
{
public:
	void OnNetJoin(ClientSocketPtr& pClient);

	void OnNetLeave(ClientSocketPtr& pClient);

	void OnNetMsg(CellServer* pCellServer, ClientSocketPtr& pClient, netmsg_DataHeader* header);


};
#endif