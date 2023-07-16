#include "CELLClient.h"

CELLClient::CELLClient(SOCKET sockfd)
	:_sendBuf(SEND_BUFF_SIZE),
	_recvBuf(RECV_BUFF_SIZE)
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

	CELLLog::Info("CELLClient::CheckHeart() dead:%d, time:%d\n", (int)_sockfd, (int)_dtHeart);

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

	//CELLLog::Info("CELLClient::CheckSend() time:%d\n", (int)_dtSend);

	if (SendDataImmediately() == SOCKET_ERROR)
		CELLLog::Info("CELLClient::SendData failed\n");

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

int CELLClient::SendData(const DataHeaderPtr& header)
{
	int nSendLen = header->len;
	const char* pSendData = (const char*)header.get();

	if (_sendBuf.Push(pSendData, nSendLen))
		return nSendLen;

	return SOCKET_ERROR;
}

int CELLClient::RecvData()
{
	return _recvBuf.Read4Socket(_sockfd);
}

bool CELLClient::NeedWrite()
{
	return _sendBuf.NeedWrite();
}

