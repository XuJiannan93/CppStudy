#ifndef _MY_CLIENT_H_
#define _MY_CLIENT_H_

#include "EasyTcpClient.h"

extern int nSendSleep;
extern int nMsg;

class MyClient : public EasyTcpClient
{
public:
	MyClient(int id = 0);

	virtual void OnNetMsg(netmsg_DataHeader* header);
	int SendTest(netmsg_Login* header);
	bool CheckSend(time_t dt);

private:
	int _id = 0;
	int _nSendMsgID = 1;
	int _nRecvMsgID = 1;
	time_t _tRestTime = 0;
	int _nSendCount = 0;
	bool _bCheckMsgID = false;

};

#endif // !_MY_CLIENT_H_
