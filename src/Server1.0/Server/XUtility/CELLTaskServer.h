#ifndef _CELL_TASK_SERVER_
#define _CELL_TASK_SERVER_

#include <thread>
#include <mutex>
#include <list>
#include <functional>

#include "Cell.hpp"
#include "CELLThread.h"

class _X_UTILITY_API_ CELLTaskServer
{
	typedef std::function<void()> CELLTask;

public:
	CELLTaskServer();
	virtual ~CELLTaskServer();

protected:
	void OnRun(CELLThread* pThread);

public:
	void AddTask(CELLTask task);
	void Start();
	void Close();

public:
	int serverID = -1;

private:
	std::list<CELLTask> _tasks;
	std::list<CELLTask> _tasksBuf;
	std::mutex _mutex;
	CELLThread _thread;
};

#endif // !_CELL_TASK_SERVER_
