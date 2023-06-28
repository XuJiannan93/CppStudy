#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>

#include "CELLTimestamp.h"

using namespace std;

mutex m;
const int tCount = 4;
atomic_int sum = 0;

void workFun(int index)
{
	for (int n = 0; n < 20000000; n++)
	{
		//m.lock();
		sum++;
		//cout << ".Hello, other thread[" << index << "]: " << sum << endl;
		//m.unlock();
	}
}

int main()
{
	thread t[3];
	for (int n = 0; n < 3; n++)
	{
		t[n] = thread(workFun, n);
		//t[n].detach();
		//t[n].join();
	}

	for (int n = 0; n < 3; n++)
	{
		//t[n] = thread(workFun, n);
		//t[n].detach();
		t[n].join();
	}

	cout << sum << endl;

	//t.join();
	//for (int n = 0; n < 4; n++)
	//	cout << "hello, main thread." << endl;

	getchar();
	return 0;
}