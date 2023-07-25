#include "CELLBuffer.h"
#include "MessageHeader.hpp"
#include "CELLLog.h"

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
	if (_nLast + len > _nSize)
	{
		_fullCount++;

		//扩展缓冲区(临时)
		//写入大量数据不要放到内存中，可以放在数据库或者磁盘中
		//int n = (_nLast + len) - _nSize;
		//if (n < 8192)
		//	n = 8192;
		//char* buf = new char[_nSize + n];
		//memcpy(buf, _pBuf, _nLast);
		//delete[] _pBuf;
		//_pBuf = buf;
	}
	else
	{
		memcpy(_pBuf + _nLast, pData, len);
		_nLast += len;

		if (_nLast == SEND_BUFF_SIZE)
		{
			_fullCount++;
		}

		return true;
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
	if (ret <= 0)
		return SOCKET_ERROR;
	if (ret == _nLast)
		_nLast = 0;
	else
	{
		_nLast -= ret;
		memcpy(_pBuf, _pBuf + ret, _nLast);
	}
	_fullCount = 0;
	return ret;
}

int CELLBuffer::Read4Socket(SOCKET sockfd)
{
	if (_nSize - _nLast <= 0)
		return 0;

	char* szRecv = _pBuf + _nLast;
	int len = (int)recv(sockfd, szRecv, _nSize - _nLast, 0);
	if (len <= 0)
	{
		CELLLog_Info("CELLBuffer::Read4Socke(), len = %d", len);
		return SOCKET_ERROR;
	}
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

bool CELLBuffer::NeedWrite()
{
	return _nLast > 0;
}
