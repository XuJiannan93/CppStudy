#ifndef _CELL_TASK_SERVER_
#define _CELL_TASK_SERVER_

#include <thread>
#include <mutex>
#include <list>
#include <functional>

#include "CELLSemaphore.h"

class CellTaskServer
{
	typedef std::function<void()> CellTask;

public:
	CellTaskServer();
	virtual ~CellTaskServer();

protected:
	void OnRun();

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
	CELLSemaphore _sem;
	bool _isRun = false;


};

#endif // !_CELL_TASK_SERVER_
