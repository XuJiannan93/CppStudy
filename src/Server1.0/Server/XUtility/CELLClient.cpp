#include "CELLClient.h"

CELLClient::CELLClient(
	SOCKET sockfd, int sendSize, int recvSize)
	:_sendBuf(sendSize),
	_recvBuf(recvSize)
{
	_sockfd = sockfd;

	ResetDTHeard();
	_ResetDTSend();
}

CELLClient::~CELLClient()
{
	if (_sockfd == INVALID_SOCKET)
		return;
#ifdef _WIN32
	closesocket(_sockfd);
#else
	close(_sockfd);
#endif // _WIN32

	_sockfd = INVALID_SOCKET;
}

void CELLClient::ResetDTHeard()
{
	_dtHeart = 0;
}

bool CELLClient::CheckHeart(time_t dt)
{
	_dtHeart += dt;
	if (_dtHeart < CLIENT_HEART_DEAD_TIME)
		return false;

	CELLLog_Info("CELLClient::CheckHeart() dead:%d, time:%d", (int)_sockfd, (int)_dtHeart);

	return true;
}

void CELLClient::_ResetDTSend()
{
	_dtSend = 0;
}

bool CELLClient::CheckSend(time_t dt)
{
	_dtSend += dt;
	if (_dtSend < CLIENT_SEND_BUFFF_TIME)
		return false;

	//CELLLog_Info("CELLClient::CheckSend() time:%d", (int)_dtSend);

	if (SendDataImmediately() == SOCKET_ERROR)
		CELLLog_Error("CELLClient::SendData failed");

	return true;
}

bool CELLClient::HasMsg()
{
	return _recvBuf.HasMsg();
}

netmsg_DataHeader* CELLClient::front_msg()
{
	return (netmsg_DataHeader*)_recvBuf.data();
}

void CELLClient::pop_front_msg()
{
	if (HasMsg())
		_recvBuf.Pop(front_msg()->len);
}

int CELLClient::SendDataImmediately()
{
	_ResetDTSend();
	return _sendBuf.Write2Socket(_sockfd);
}

SOCKET CELLClient::sockfd()
{
	return _sockfd;
}

int CELLClient::SendData(const char* data, int len)
{
	if (_sendBuf.Push(data, len))
		return len;
	return SOCKET_ERROR;
}

int CELLClient::SendData(const DataHeaderPtr& header)
{
	return SendData((const char*)header.get(), header->len);
}

int CELLClient::RecvData()
{
	return _recvBuf.Read4Socket(_sockfd);
}

bool CELLClient::NeedWrite()
{
	return _sendBuf.NeedWrite();
}

