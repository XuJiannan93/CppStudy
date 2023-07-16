#ifndef _ALLOCTOR_HPP_
#define _ALLOCTOR_HPP_

#include "MemoryMgr.h"

void* operator new(size_t size);
void operator delete(void* p);
void* operator new[](size_t size);
void operator delete[](void* p);
void* mem_alloc(size_t size);
void mem_free(void* p);

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

#endif // !_ALLOCTOR_HPP_

