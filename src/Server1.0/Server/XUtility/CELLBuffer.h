#ifndef _CELL_BUFFER_H_
#define _CELL_BUFFER_H_

#include "Cell.hpp"

class _X_UTILITY_API_ CELLBuffer
{
public:
	CELLBuffer(int size = 8192);
	~CELLBuffer();

	char* data();
	bool Push(const char* pData, int len);
	void Pop(int len);
	int Write2Socket(SOCKET sockfd);
	int Read4Socket(SOCKET sockfd);
	bool HasMsg();

private:
	char* _pBuf = nullptr;
	int _nLast = 0;
	int _nSize = 0;
	int _fullCount = 0;
};

#endif // !_CELL_BUFFER_H_
