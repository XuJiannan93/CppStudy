#ifndef _MEMORY_ALLOC_H_
#define _MEMORY_ALLOC_H_

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <mutex>

#include "XPrintf.h"
#include "MemoryBlock.h"

class MemoryAlloc
{
public:
	MemoryAlloc();
	virtual ~MemoryAlloc();

	void InitMemory(MemoryAlloc* mem);

	void* memory_alloc(size_t size);
	void memory_free(void* p);

private:
	std::mutex _mutex;

protected:
	char* _pBuf;
	MemoryBlock* _pHeader;
	size_t _nSize;
	size_t _nBlockCount;
};


template<size_t nSize, size_t nBlockCount>
class MemoryAlloctor : public MemoryAlloc
{
public:
	MemoryAlloctor()
	{
		const size_t n = sizeof(void*);
		_nSize = ((nSize / n) * n) + (nSize % n ? n : 0);
		_nBlockCount = nBlockCount;
	}
};

#endif // ! _MEMORY_ALLOC_H_

