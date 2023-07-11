#ifndef _CELL_TASK_SERVER_
#define _CELL_TASK_SERVER_

#include <thread>
#include <mutex>
#include <list>
#include <functional>

#include "CELLThread.h"

class CELLTaskServer
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
	//CELLSemaphore _sem;
	//bool _isRun = false;


};

#endif // !_CELL_TASK_SERVER_
