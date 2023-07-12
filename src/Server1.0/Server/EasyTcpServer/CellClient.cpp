#include "CELLClient.h"

CELLClient::CELLClient(SOCKET sockfd)
{
	_sockfd = sockfd;
	memset(_szMsgBuf, 0, RECV_BUFF_SIZE);
	memset(_szSendBuf, 0, SEND_BUFF_SIZE);
	_lastPos = 0;
	_lastSendPos = 0;

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

	printf("CELLClient::CheckHeart() dead:%d, time:%d\n", (int)_sockfd, (int)_dtHeart);

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

	//printf("CELLClient::CheckSend() time:%d\n", (int)_dtSend);

	if (SendDataImmediately() == SOCKET_ERROR)
		printf("CELLClient::SendData failed\n");

	return true;
}

int CELLClient::SendDataImmediately()
{
	if (_sockfd == INVALID_SOCKET) return 0;
	if (_lastSendPos <= 0) return 0;

	auto ret = send(_sockfd, _szSendBuf, _lastSendPos, 0);
	_lastSendPos = 0;
	_sendBuffFullCount = 0;
	_ResetDTSend();

	return ret;
}

SOCKET CELLClient::sockfd()
{
	return _sockfd;
}

char* CELLClient::msgBuf()
{
	return _szMsgBuf;
}

int CELLClient::getLastPos()
{
	return _lastPos;
}

void CELLClient::setLastPos(int pos)
{
	_lastPos = pos;
}

int CELLClient::SendData(const DataHeaderPtr& header)
{
	int ret = SOCKET_ERROR;
	int nSendLen = header->len;
	const char* pSendData = (const char*)header.get();

	if (_lastSendPos + nSendLen <= SEND_BUFF_SIZE)
	{
		memcpy(_szSendBuf + _lastSendPos, pSendData, nSendLen);
		_lastSendPos += nSendLen;

		if (_lastSendPos == SEND_BUFF_SIZE)
		{
			_sendBuffFullCount++;
		}

		return nSendLen;
	}
	else
	{
		_sendBuffFullCount++;
	}

	return ret;
}
