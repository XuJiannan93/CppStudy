#ifndef MY_SERVER_H
#define MY_SERVER_H

#include "MessageHeader.hpp"
#include "EasyTcpServer.h"

class MyServer : public EasyTcpServer
{
public:
	void OnNetJoin(ClientSocket* pClient) override;

	void OnNetLeave(ClientSocket* pClient) override;

	void OnNetMsg(ClientSocket* pClient, DataHeader* header) override;


};
#endif