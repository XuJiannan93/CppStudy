#ifndef _CELL_THREAD_H_
#define _CELL_THREAD_H_

#include <functional>

#include "Cell.hpp"
#include "CELLSemaphore.h"

class _X_UTILITY_API_ CELLThread
{
private:
	typedef std::function<void(CELLThread*)> EventCall;

public:
	CELLThread();
	~CELLThread();

	void Start(
		EventCall onCreate = nullptr,
		EventCall onRun = nullptr,
		EventCall onDestroy = nullptr);
	void Close();
	void Exit();
	bool isRun();

protected:
	void OnWork();

private:
	EventCall _onCreate;
	EventCall _onRun;
	EventCall _onDestroy;

	std::mutex _mutex;
	CELLSemaphore _sem;
	bool _isRun = false;

public:
	static void Sleep(time_t dt);

};

#endif // !_CELL_THREAD_H_
