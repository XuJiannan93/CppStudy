#ifndef _MEMORY_BLOCK_H_
#define _MEMORY_BLOCK_H_

extern class MemoryAlloc;

class MemoryBlock
{
public:
	int nID;
	int nRef;
	MemoryAlloc* pAlloc;
	MemoryBlock* pNext;
	bool bPool;

private:

public:
	MemoryBlock();
	~MemoryBlock();
};

#endif // ! _MEMORY_BLOCK_H_

