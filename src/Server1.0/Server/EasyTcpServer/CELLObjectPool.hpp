#ifndef _CELL_OBJECT_POOL_HPP_
#define _CELL_OBJECT_POOL_HPP_

#include <stdlib.h>
#include <mutex>
#include <assert.h>
#include <iostream>

#include "XPrintf.h"

template<class Type, size_t nPoolSize>
class CellObjectPool
{
private:
	struct NodeHeader
	{
		NodeHeader* pNext;
		int nID;
		char nRef;
		bool bPool;
	};

private:
	NodeHeader* _pHeader;
	char* _pBuf;
	std::mutex _mutex;

private:
	void initPool()
	{
		if (_pBuf) return;

		size_t realSize = sizeof(Type) + sizeof(NodeHeader);
		size_t n = nPoolSize * realSize;

		_pBuf = new char[n];

		_pHeader = (NodeHeader*)_pBuf;
		_pHeader->bPool = true;
		_pHeader->nID = 0;
		_pHeader->nRef = 0;
		_pHeader->pNext = nullptr;

		NodeHeader* pTemp1 = _pHeader;
		for (size_t n = 1; n < nPoolSize; n++)
		{
			NodeHeader* pTemp2 = (NodeHeader*)(_pBuf + (n * realSize));
			pTemp2->bPool = true;
			pTemp2->nID = n;
			pTemp2->nRef = 0;
			pTemp2->pNext = nullptr;
			pTemp1->pNext = pTemp2;
			pTemp1 = pTemp2;
		}
	}

public:

	CellObjectPool()
	{
		_pBuf = nullptr;
		initPool();
	}

	~CellObjectPool()
	{
		if (_pBuf)
			delete[] _pBuf;
	}

	void* AllocObjMemory(size_t size)
	{
		std::lock_guard<std::mutex> lg(_mutex);
		NodeHeader* pReturn = nullptr;
		if (_pHeader == nullptr)
		{
			pReturn = (NodeHeader*)new char[sizeof(Type) + sizeof(NodeHeader)];
			pReturn->bPool = false;
			pReturn->nID = -1;
			pReturn->nRef = 1;
			pReturn->pNext = nullptr;
		}
		else
		{
			pReturn = _pHeader;
			_pHeader = _pHeader->pNext;
			assert(pReturn->nRef == 0);
			pReturn->nRef = 1;
		}

		xPrintf("alloc obj memory : %llx, id = %d, size = %d\n", pReturn, pReturn->nID, size);
		return ((char*)pReturn + sizeof(NodeHeader));
	}

	void FreeObjMemory(void* pObj)
	{
		if (pObj == nullptr)
		{
			printf("CellObjectPool::FreeObjMemory: nullptr!!\n");
			return;
		}

		NodeHeader* pBlock = (NodeHeader*)((char*)pObj - sizeof(NodeHeader));

		assert(pBlock->nRef == 1);

		xPrintf("free obj memory : %llx, id = %d\n", pBlock, pBlock->nID);

		if (pBlock->bPool)
		{
			std::lock_guard<std::mutex> lg(_mutex);
			if (--pBlock->nRef != 0)
			{
				return;
			}

			pBlock->pNext = _pHeader;
			_pHeader = pBlock;
		}
		else
		{
			if (--pBlock->nRef != 0)
			{
				return;
			}
			delete[] pBlock;
		}
	}
};



#endif // !_CELL_OBJECT_POOL_HPP_
