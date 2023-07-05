#include "MemoryMgr.h"

MemoryMgr::MemoryMgr()
{
	init_szAlloc(0, 64, &_mem64);
	init_szAlloc(65, 128, &_mem128);
	init_szAlloc(129, 256, &_mem256);
	init_szAlloc(257, 512, &_mem512);
	init_szAlloc(513, 1024, &_mem1024);
}

MemoryMgr::~MemoryMgr()
{
}

MemoryMgr& MemoryMgr::Instance()
{
	static MemoryMgr mgr;
	return mgr;
}

void MemoryMgr::init_szAlloc(int nBegin, int nEnd, MemoryAlloc* pMem)
{
	_szAlloc[nBegin]->InitMemory(pMem);

	for (int n = nBegin; n <= nEnd; n++)
	{
		_szAlloc[n] = pMem;
	}
}

void* MemoryMgr::mem_alloc(size_t nSize)
{
	if (nSize <= MAX_MEMORY_SIZE)
		return _szAlloc[nSize]->memory_alloc(nSize);
	else
	{
		MemoryBlock* pReturn = (MemoryBlock*)malloc(nSize + sizeof(MemoryBlock));
		pReturn->bPool = false;
		pReturn->nID = -1;
		pReturn->nRef = 1;
		pReturn->pAlloc = nullptr;
		pReturn->pNext = nullptr;
		//xPrintf("MemoryMgr::mem_alloc: [%llx] id=%d, size=%d\n", pReturn, pReturn->nID, nSize);

		return (char*)pReturn + sizeof(MemoryBlock);
	}
}

void MemoryMgr::mem_free(void* pMem)
{
	MemoryBlock* pBlock = (MemoryBlock*)((char*)pMem - sizeof(MemoryBlock));
	//xPrintf("MemoryMgr::mem_alloc: [%llx] id=%d\n", pBlock, pBlock->nID);

	if (pBlock->bPool)
		pBlock->pAlloc->memory_free(pMem);
	else
	{
		if (--pBlock->nRef != 0)
			return;

		free(pBlock);
	}
}

void MemoryMgr::addRef(void* pMem)
{
	MemoryBlock* pBlock = (MemoryBlock*)((char*)pMem - sizeof(MemoryBlock));
	++pBlock->nRef;
}
