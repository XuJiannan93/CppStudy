#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#include <stdlib.h>

#include "XPrintf.h"
#include "MemoryAlloc.h"

#define MAX_MEMORY_SIZE 1024 

class MemoryMgr
{
private:
	MemoryMgr();
	virtual ~MemoryMgr();

	void init_szAlloc(int nBegin, int nEnd, MemoryAlloc* pMem);
	void addRef(void* pMem);

public:
	static MemoryMgr& Instance();
	void* mem_alloc(size_t nSize);
	void mem_free(void* p);

private:
	MemoryAlloctor<64, 100000> _mem64;
	MemoryAlloctor<128, 100000> _mem128;
	MemoryAlloctor<256, 100000> _mem256;
	MemoryAlloctor<512, 100000> _mem512;
	MemoryAlloctor<1024, 100000> _mem1024;

	MemoryAlloc* _szAlloc[MAX_MEMORY_SIZE + 1];

};

#endif // !_MEMORY_MANAGER_H_
