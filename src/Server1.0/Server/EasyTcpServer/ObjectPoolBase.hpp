#ifndef _OBJECT_POOL_BASE_HPP_
#define _OBJECT_POOL_BASE_HPP_

#include "CELLObjectPool.hpp"

template<class Type, size_t nPoolSize>
class ObjectPoolBase
{
public:
	void* operator new(size_t size)
	{
		return objectPool().AllocObjMemory(size);
	}

	void operator delete(void* p)
	{
		objectPool().FreeObjMemory(p);
	}

	template<typename ...Args>
	static Type* CreateObject(Args ... args)
	{
		auto obj = new Type(args...);
		return obj;
	}

	static void DestroyObject(Type* obj)
	{
		delete obj;
	}

private:
	typedef CELLObjectPool<Type, nPoolSize> ClassTypePool;

	static ClassTypePool& objectPool()
	{
		static ClassTypePool sPool;
		return sPool;
	}


};
#endif // !_OBJECT_POOL_BASE_HPP_
