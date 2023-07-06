#include <stdlib.h>

#include "Alloctor.h"
#include "MemoryMgr.h"

void* operator new(size_t size)
{
	return MemoryMgr::Instance().mem_alloc(size);
}

void operator delete(void* p)
{
	MemoryMgr::Instance().mem_free(p);
}

void* operator new[](size_t size)
{
	return MemoryMgr::Instance().mem_alloc(size);
}

void operator delete[](void* p)
{
	MemoryMgr::Instance().mem_free(p);
}

void* mem_alloc(size_t size)
{
	return MemoryMgr::Instance().mem_alloc(size);
}

void mem_free(void* p)
{
	MemoryMgr::Instance().mem_free(p);
}
