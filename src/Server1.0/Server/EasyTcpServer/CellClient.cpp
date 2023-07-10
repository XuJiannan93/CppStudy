#include "CellClient.h"

CellClient::CellClient(SOCKET sockfd)
{
	_sockfd = sockfd;
	memset(_szMsgBuf, 0, RECV_BUFF_SIZE);
	memset(_szSendBuf, 0, SEND_BUFF_SIZE);
	_lastPos = 0;
	_lastSendPos = 0;
}

CellClient::~CellClient()
{
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

int CellClient::SendData(DataHeaderPtr& header)
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