#include "CELLThread.h"

CELLThread::CELLThread()
{
}

CELLThread::~CELLThread()
{
}

void CELLThread::Start(
	EventCall onCreate,
	EventCall onRun,
	EventCall onDestroy)
{
	if (_isRun) return;

	std::lock_guard<std::mutex> lock(_mutex);

	if (onCreate)
		_onCreate = onCreate;

	if (onRun)
		_onRun = onRun;

	if (onDestroy)
		_onDestroy = onDestroy;

	_isRun = true;
	std::thread t(std::mem_fn(&CELLThread::OnWork), this);
	t.detach();

}

void CELLThread::Close()
{
	if (_isRun == false) return;

	std::lock_guard<std::mutex> lock(_mutex);

	_isRun = false;
	_sem.Wait();
}

void CELLThread::Exit()
{
	if (_isRun == false) return;

	std::lock_guard<std::mutex> lock(_mutex);

	_isRun = false;
}

void CELLThread::OnWork()
{
	if (_onCreate)
	{
		_onCreate(this);
	}
	if (_onRun)
		_onRun(this);
	if (_onDestroy)
		_onDestroy(this);

	_sem.Wakeup();
}

bool CELLThread::isRun()
{
	return _isRun;
}

void CELLThread::Sleep(time_t dt)
{
	std::chrono::milliseconds t(dt);
	std::this_thread::sleep_for(t);
}
