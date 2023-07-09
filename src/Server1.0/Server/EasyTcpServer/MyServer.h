#ifndef MY_SERVER_H
#define MY_SERVER_H

#include "MessageHeader.hpp"
#include "EasyTcpServer.h"
#include "CellS2CTask.h"


class MyServer : public EasyTcpServer
{
public:
	void OnNetJoin(ClientSocketPtr& pClient);

	void OnNetLeave(ClientSocketPtr& pClient);

	void OnNetMsg(CellServer* pCellServer, ClientSocketPtr& pClient, DataHeader* header);


};
#endif