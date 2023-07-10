#include "CellClient.h"

CellClient::CellClient(SOCKET sockfd)
{
	_sockfd = sockfd;
	memset(_szMsgBuf, 0, RECV_BUFF_SIZE);
	memset(_szSendBuf, 0, SEND_BUFF_SIZE);
	_lastPos = 0;
	_lastSendPos = 0;

	ResetDTHeard();
	_ResetDTSend();
}

CellClient::~CellClient()
{
}

void CellClient::ResetDTHeard()
{
	_dtHeart = 0;
}

bool CellClient::CheckHeart(time_t dt)
{
	_dtHeart += dt;
	if (_dtHeart < CLIENT_HEART_DEAD_TIME)
		return false;

	printf("CellClient::CheckHeart() dead:%d, time:%d\n", (int)_sockfd, (int)_dtHeart);

	return true;
}

void CellClient::_ResetDTSend()
{
	_dtSend = 0;
}

bool CellClient::CheckSend(time_t dt)
{
	_dtSend += dt;
	if (_dtSend < CLIENT_SEND_BUFFF_TIME)
		return false;

	printf("CellClient::CheckSend() time:%d\n", (int)_dtSend);

	if (_SendDataImmediately() == SOCKET_ERROR)
		printf("CellClient::SendData failed\n");

	return true;
}

int CellClient::_SendDataImmediately()
{
	if (_sockfd == SOCKET_ERROR) return SOCKET_ERROR;
	if (_lastSendPos = 0) return SOCKET_ERROR;

	auto ret = send(_sockfd, _szSendBuf, _lastSendPos, 0);
	_lastSendPos = 0;
	_ResetDTSend();

	return ret;
}

SOCKET CellClient::sockfd()
{
	return _sockfd;
}

char* CellClient::msgBuf()
{
	return _szMsgBuf;
}

int CellClient::getLastPos()
{
	return _lastPos;
}
void CellClient::setLastPos(int pos)
{
	_lastPos = pos;
}

int CellClient::SendData(const DataHeaderPtr& header)
{
	int ret = SOCKET_ERROR;
	int nSendLen = header->len;
	const char* pSendData = (const char*)header.get();

	while (true)
	{
		if (_lastSendPos + nSendLen >= SEND_BUFF_SIZE)
		{
			int nCopyLen = SEND_BUFF_SIZE - _lastSendPos;
			memcpy(_szSendBuf + _lastSendPos, pSendData, nCopyLen);
			pSendData += nCopyLen;
			nSendLen -= nCopyLen;
			ret = send(_sockfd, _szSendBuf, SEND_BUFF_SIZE, 0);
			_lastSendPos = 0;
			_ResetDTSend();
			if (ret == SOCKET_ERROR)
				return ret;
		}
		else
		{
			memcpy(_szSendBuf + _lastSendPos, pSendData, nSendLen);
			_lastSendPos += nSendLen;
			break;
		}
	}

	return ret;
}
