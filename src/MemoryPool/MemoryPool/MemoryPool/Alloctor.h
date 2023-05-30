#pragma once

void* operator new(size_t nSize);

void operator delete(void* pMem);

void* operator new[](size_t nSize);

void operator delete[](void* pMen);

void* mem_alloc(size_t nSize);

void mem_free(void* pMen);
