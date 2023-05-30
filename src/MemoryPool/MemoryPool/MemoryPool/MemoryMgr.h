#pragma once

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

class MemoryAlloc;
class MemoryBlock
{
public:
	size_t	nRef;
	MemoryAlloc* pAlloc;
	MemoryBlock* pNext;
	bool bPool;
	char cReserver1;
	char cReserver2;
	char cReserver3;
};

class MemoryAlloc
{
public:
	MemoryAlloc()
	{
		m_pBuf = 0;
		m_nInitBlock = 1024;
	}

	~MemoryAlloc()
	{
		_freeNotInPool();
		if (m_pBuf != 0)
		{
			::free(m_pBuf);
		}
	}

	void* allocMemory(size_t nSize)
	{
		if (m_pBuf == 0)
		{
			_initMemory();
		}

		MemoryBlock* pReturn = 0;

		if (m_pHeader == 0)
		{
			pReturn = (MemoryBlock*)(::malloc(nSize + sizeof(MemoryBlock)));
			pReturn->bPool = false;
			pReturn->nRef = 1;
			pReturn->pAlloc = this;
			pReturn->pNext = 0;
		}
		else
		{
			pReturn = m_pHeader;
			pReturn = m_pHeader->pNext;
			pReturn->pAlloc = this;
			pReturn->bPool = true;
			assert(pReturn->nRef == 0);
			pReturn->nRef = 1;
		}
		return ((char*)pReturn) + sizeof(MemoryBlock);
	}

	void freeMemory(void* pMem)
	{
		char* pCh = (char*)pMem;
		MemoryBlock* pMap = (MemoryBlock*)((char*)(pCh - sizeof(MemoryBlock)));

		if (--pMap->nRef != 0)
			return;

		if (!pMap->bPool)
		{
			::free(pMap);
			return;
		}

		pMap->pNext = m_pHeader;
		m_pHeader = pMap;
	}

	void _initMemory()
	{
		assert(m_pBuf == 0);

		if (m_pBuf != 0)
			return;

		size_t nBuf = m_nSize * m_nInitBlock;
		m_pBuf = (char*)::malloc(nBuf);

		m_pHeader = (MemoryBlock*)m_pBuf;
		m_pHeader->nRef = 0;
		m_pHeader->pAlloc = this;
		m_pHeader->pNext = 0;
		m_pHeader->bPool = true;
		MemoryBlock* pTemp = m_pHeader;

		for (size_t i = 1; i < m_nInitBlock; ++i)
		{
			char* pBlock = (m_pBuf + m_nSize * i);
			MemoryBlock* pMap = (MemoryBlock*)pBlock;
			pMap->nRef = 0;
			pMap->bPool = true;
			pMap->pAlloc = this;
			pMap->pNext = 0;
			pTemp->pNext = pMap;
			pTemp = pMap;
		}
	}

	void _freeNotInPool()
	{
		while (m_pHeader != 0)
		{
			MemoryBlock* pHeader = m_pHeader;
			m_pHeader = m_pHeader->pNext;
			if (!pHeader->bPool)
			{
				::free(pHeader);
			}
		}
		m_pHeader = 0;
	}

	size_t _getFreeBlock()
	{
		MemoryBlock* pTemp = m_pHeader;
		size_t nSize = 0;
		while (pTemp != 0)
		{
			++nSize;
			pTemp = pTemp->pNext;
		}
		return nSize;
	}

protected:
	char* m_pBuf;
	MemoryBlock* m_pHeader;
	size_t m_nInitBlock;
	size_t m_nSize;
};

template<size_t nBlock, size_t nInitSize>
class MemoryAlloctor :public MemoryAlloc
{
public:
	MemoryAlloctor()
	{
		m_nInitBlock = nInitSize;
		m_nSize = nBlock / sizeof(void*) * sizeof(void*) + (nBlock % sizeof(void*) ? sizeof(void*) : 0);
		m_nsize = m_nSize + sizeof(memoryBlock);
	}
};

class MemoryMgr
{
public:
	MemoryMgr(void)
	{
		init(0, 64, &m_mem64);
		init(64, 128, &m_mem128);
		init(128, 256, &m_mem256);
		init(256, 512, &m_mem512);
		init(512, 1024, &m_mem1024);
		init(1024, 2048, &m_mem2048);
		init(2048, 4096, &m_mem4096);
	}

	~MemoryMgr()
	{
	}

	static MemoryMgr& instance()
	{
		static MemoryMgr sInstance;
		return sInstance;
	}

	void* alloc(size_t nSize)
	{
		if (nSize < sizeof(m_arAlloc) / sizeof(MemoryAlloc*))
		{
			return m_arAlloc[nSize]->allocMemory(nSize);
		}
		else
		{
			char* pMem = (char*)::malloc(nSize + sizeof(MemoryBlock));
			MemoryBlock* pBlock = (MemoryBlock*)pMem;
			pBlock->bPool = false;
			pBlock->nRef = 1;
			pBlock->pAlloc = 0;
			pBlock->pNext = 0;
			return pMem + sizeof(MemoryBlock);
		}
	}

	void free(void* pMem)
	{
		char* pCh = (char*)pMem;
		MemoryBlock* pMap = (MemoryBlock*)(char*)(pCh - sizeof(MemoryBlock));
		if (pMap->bPool)
		{
			pMap->pAlloc->freeMemory(pMem);
		}
		else if (--pMap->nRef == 0)
		{
			::free(pMap);
		}
	}

	void addRef(void* pMem)
	{
		MemoryBlock* pMap = (MemoryBlock*)((char*)pMem - sizeof(MemoryBlock));
		++pMap->nRef;
	}

protected:
	void init(size_t nBegin, size_t nEnd, MemoryAlloc* pAlloc)
	{
		for (size_t i = nBegin; i < nEnd; ++i)
		{
			m_arAlloc[i] = pAlloc;

		}
	}

protected:
	MemoryAlloctor<64, 10240> m_mem64;
	MemoryAlloctor<128, 10240> m_mem128;
	MemoryAlloctor<256, 10240> m_mem256;
	MemoryAlloctor<512, 10240> m_mem512;
	MemoryAlloctor<1025, 10240> m_mem1024;
	MemoryAlloctor<2048, 10240> m_mem2048;
	MemoryAlloctor<4096, 10240> m_mem4096;

	MemoryAlloc* m_arAlloc[4096];
};
