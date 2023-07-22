#ifndef _UNITY_TCP_CLIENT_H_
#define _UNITY_TCP_CLIENT_H_

#include "Header.h"
#include "EasyTcpClient.h"
#include "MessageHeader.hpp"

class UnityTcpClient : public EasyTcpClient
{
public:
	virtual void OnNetMsg(netmsg_DataHeader* header);
	void SetCallback(void* obj, OnNetMsgCallback cb);

private:
	void* _csObj = nullptr;
	OnNetMsgCallback _callback = nullptr;
};

#endif // !_UNITY_TCP_CLIENT_H_
