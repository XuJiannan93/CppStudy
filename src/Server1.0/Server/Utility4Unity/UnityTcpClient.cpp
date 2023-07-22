#include "UnityTcpClient.h"

void UnityTcpClient::OnNetMsg(netmsg_DataHeader* header)
{
	if (_callback == nullptr)
		return;

	_callback(_csObj, header, header->len);
}

void UnityTcpClient::SetCallback(void* obj, OnNetMsgCallback cb)
{
	_csObj = obj;
	_callback = cb;
}
