#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>

#include "Alloctor.h"
#include "CELLTimestamp.h"
#include "ObjectPoolBase.hpp"

using namespace std;

mutex m;
const int tCount = 8;
const int mCount = 100000;
const int nCount = mCount / tCount;

class ClassA : public ObjectPoolBase<ClassA, 100000>
{
public:
	ClassA(int n);

	~ClassA();

public:
	int num = 0;
};

ClassA::ClassA(int n)
{
	num = n;
	printf("Class(%d)", n);
}

ClassA::~ClassA()
{
	printf("~Class()");
}

void workFun(int index)
{
	//char* data[nCount];
	ClassA* data[nCount];
	for (size_t i = 0; i < nCount; i++)
	{
		//data[i] = new char[rand() % 128 + 1];
		data[i] = ClassA::CreateObject(i);
	}

	for (size_t i = 0; i < nCount; i++)
	{
		//delete[] data[i];
		printf("delete data[%d]", data[i]->num);
		ClassA::DestroyObject(data[i]);
	}
}

int main()
{
	//thread t[3];
	//CELLTimestamp tTime;
	//for (int n = 0; n < 3; n++)
	//{
	//	t[n] = thread(workFun, n);
	//}

	//for (int n = 0; n < 3; n++)
	//{
	//	t[n].join();
	//}

	//cout << tTime.GetElapsedTimeInMilliSec() << endl;

	//int* a = new int;
	//delete a;

	{
		std::shared_ptr<ClassA> b = std::make_shared<ClassA>(5);
	}
	{
		std::shared_ptr<ClassA> ptr1(new ClassA(6));
		std::shared_ptr<ClassA> ptr2(new ClassA(7));
		std::shared_ptr<ClassA> ptr3(new ClassA(8));
	}

	//ClassA* a1 = new ClassA(1);
	//delete a1;

	//ClassA* a2 = ClassA::CreateObject(2);
	//ClassA::DestroyObject(a2);

	return 0;
}