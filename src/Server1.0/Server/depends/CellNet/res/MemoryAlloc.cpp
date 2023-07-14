#include "MemoryAlloc.h"

MemoryAlloc::MemoryAlloc()
{
	_pBuf = nullptr;
	_pHeader = nullptr;
	_nSize = 0;
	_nBlockCount = 0;
}

MemoryAlloc::~MemoryAlloc()
{
	if (_pBuf == nullptr)
		return;
	free(_pBuf);
}

void MemoryAlloc::InitMemory(MemoryAlloc* mem)
{
	xPrintf("MemoryAlloc::InitMemory[%d]\n", mem->_nSize);

	if (mem->_pBuf != nullptr) return;

	size_t blocksize = mem->_nSize + sizeof(MemoryBlock);
	size_t buffsize = blocksize * mem->_nBlockCount;

	mem->_pBuf = (char*)malloc(buffsize);

	mem->_pHeader = (MemoryBlock*)mem->_pBuf;
	mem->_pHeader->bPool = true;
	mem->_pHeader->nID = 0;
	mem->_pHeader->nRef = 0;
	mem->_pHeader->pAlloc = mem;
	mem->_pHeader->pNext = nullptr;

	MemoryBlock* pTemp1 = mem->_pHeader;
	for (size_t n = 1; n < mem->_nBlockCount; n++)
	{
		MemoryBlock* pTemp2 = (MemoryBlock*)(mem->_pBuf + n * blocksize);
		pTemp2->bPool = true;
		pTemp2->nID = n;
		pTemp2->nRef = 0;
		pTemp2->pAlloc = mem;
		pTemp2->pNext = nullptr;
		pTemp1->pNext = pTemp2;
		pTemp1 = pTemp2;
	}
}

void* MemoryAlloc::memory_alloc(size_t size)
{
	std::lock_guard<std::mutex> lg(_mutex);

	assert(_pBuf != nullptr);

	MemoryBlock* pReturn = nullptr;

	if (_pHeader == nullptr)
	{
		pReturn = (MemoryBlock*)malloc(size + sizeof(MemoryBlock));
		if (pReturn == nullptr)
		{
			xPrintf("MemoryAlloc::malloc failed: [%llx], size[%d]\n", pReturn, size);
			return nullptr;
		}

		pReturn->bPool = false;
		pReturn->nID = -1;
		pReturn->nRef = 1;
		pReturn->pAlloc = nullptr;
		pReturn->pNext = nullptr;

		xPrintf("MemoryAlloc::mem_alloc: [%llx], size[%d]\n", pReturn, size);
	}
	else
	{
		pReturn = _pHeader;
		_pHeader = _pHeader->pNext;
		assert(pReturn->nRef == 0);
		pReturn->nRef = 1;
	}

	xPrintf("MemoryAlloc::mem_alloc: [%llx] id=%d, size=%d\n", pReturn, pReturn->nID, size);
	return (char*)pReturn + sizeof(MemoryBlock);
}

void MemoryAlloc::memory_free(void* pMem)
{
	MemoryBlock* pBlock = (MemoryBlock*)((char*)pMem - sizeof(MemoryBlock));
	//xCELLLog::Info("MemoryAlloc::mem_free: [%llx] id=%d\n", pBlock, pBlock->nID);
	assert(pBlock->nRef == 1);

	if (pBlock->bPool == false)
	{
		if (--pBlock->nRef != 0)
			return;

		free(pBlock);
		return;
	}

	std::lock_guard<std::mutex> lg(_mutex);
	if (--pBlock->nRef != 0)
		return;
	pBlock->pNext = pBlock->pAlloc->_pHeader;
	pBlock->pAlloc->_pHeader = pBlock;
}

