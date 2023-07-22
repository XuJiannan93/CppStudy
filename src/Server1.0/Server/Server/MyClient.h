#ifndef _MY_CLIENT_H_
#define _MY_CLIENT_H_

#include "EasyTcpClient.h"

class MyClient : public EasyTcpClient
{
public:
	virtual void OnNetMsg(netmsg_DataHeader* header);

};

#endif // !_MY_CLIENT_H_
