#ifndef _CELL_SEMAPHORE_H_
#define _CELL_SEMAPHORE_H_

#include <mutex>
#include <condition_variable>

class __declspec(dllexport) CELLSemaphore
{
public:
	CELLSemaphore();
	~CELLSemaphore();

	void Wait();
	void Wakeup();

private:
	std::mutex _mutex;
	std::condition_variable _cv;
	int _wait = 0;
	int _wakeup = 0;
};

#endif // !_CELL_SEMAPHORE_H_
