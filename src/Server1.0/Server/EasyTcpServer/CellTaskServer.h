#ifndef _CELL_TASK_SERVER_
#define _CELL_TASK_SERVER_

#include <thread>
#include <mutex>
#include <list>
#include <functional>

#include "CELLThread.h"

class CellTaskServer
{
	typedef std::function<void()> CellTask;

public:
	CellTaskServer();
	virtual ~CellTaskServer();

protected:
	void OnRun(CELLThread* pThread);

public:
	void AddTask(CellTask task);
	void Start();
	void Close();

public:
	int serverID = -1;

private:
	std::list<CellTask> _tasks;
	std::list<CellTask> _tasksBuf;
	std::mutex _mutex;
	CELLThread _thread;
	//CELLSemaphore _sem;
	//bool _isRun = false;


};

#endif // !_CELL_TASK_SERVER_
