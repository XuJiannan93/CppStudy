#include "CELLSemaphore.h"

#include <chrono>
#include <thread>

CELLSemaphore::CELLSemaphore()
{
}

CELLSemaphore::~CELLSemaphore()
{
}

void CELLSemaphore::Wait()
{
	std::unique_lock<std::mutex> lock(_mutex);
	if (--_wait < 0)
	{
		_cv.wait(lock, [this]()->bool {
			return _wakeup > 0;
			});
		--_wakeup;
	}
}

void CELLSemaphore::Wakeup()
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (++_wait > 0)
		return;

	++_wakeup;
	_cv.notify_one();
	return;
}