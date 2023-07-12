#include "CELLBuffer.h"
#include "MessageHeader.hpp"

CELLBuffer::CELLBuffer(int size)
{
	_nSize = size;
	_pBuf = new char[_nSize];
}

CELLBuffer::~CELLBuffer()
{
	if (_pBuf == nullptr)
		return;

	delete[] _pBuf;
	_pBuf = nullptr;
}

char* CELLBuffer::data()
{
	return _pBuf;
}

bool CELLBuffer::Push(const char* pData, int len)
{
	if (_nLast + len <= _nSize)
	{
		memcpy(_pBuf + _nLast, pData, len);
		_nLast += len;

		if (_nLast == SEND_BUFF_SIZE)
		{
			_fullCount++;
		}

		return true;
	}
	else
	{
		_fullCount++;
	}

	return false;
}

void CELLBuffer::Pop(int len)
{
	int n = _nLast - len;
	if (n > 0)
		memcpy(_pBuf, _pBuf + len, n);

	_nLast = n;

	if (_fullCount > 0)
		_fullCount--;
}

int CELLBuffer::Write2Socket(SOCKET sockfd)
{
	if (_nLast <= 0) return 0;
	if (sockfd == INVALID_SOCKET) return 0;

	int ret = send(sockfd, _pBuf, _nLast, 0);
	_nLast = 0;
	_fullCount = 0;

	return ret;
}

int CELLBuffer::Read4Socket(SOCKET sockfd)
{
	if (_nSize - _nLast <= 0)
		return 0;

	char* szRecv = _pBuf + _nLast;
	int len = (int)recv(sockfd, szRecv, _nSize - _nLast, 0);
	_nLast += len;
	return len;
}

bool CELLBuffer::HasMsg()
{
	if (_nLast < sizeof(netmsg_DataHeader))
		return false;

	netmsg_DataHeader* header = (netmsg_DataHeader*)_pBuf;

	return _nLast >= header->len;
}
