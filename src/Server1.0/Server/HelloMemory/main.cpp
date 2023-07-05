//#include <stdlib.h>
//
//#include <string.h>
//#include <sstream>
//#include <iostream>
//
//int main()
//{
//	char* data[100];
//	for (size_t i = 0; i < 100; i++)
//	{
//		data[i] = new char[60];
//	}
//
//	for (size_t i = 0; i < 100; i++)
//	{
//		delete[] data[i];
//	}
//
//
//	return 0;
//}

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>

//#include "Alloctor.h"
#include "CELLTimestamp.h"

using namespace std;

mutex m;
const int tCount = 8;
const int mCount = 100000;
const int nCount = mCount / tCount;

void workFun(int index)
{
	char* data[nCount];
	for (size_t i = 0; i < nCount; i++)
	{
		data[i] = new char[rand() % 128 + 1];
	}

	for (size_t i = 0; i < nCount; i++)
	{
		delete[] data[i];
	}
	//for (int n = 0; n < nCount; n++)
	//{
	//}
}

int main()
{
	thread t[3];
	CELLTimestamp tTime;
	for (int n = 0; n < 3; n++)
	{
		t[n] = thread(workFun, n);
	}

	for (int n = 0; n < 3; n++)
	{
		t[n].join();
	}

	cout << tTime.GetElapsedTimeInMilliSec() << endl;

	return 0;
}