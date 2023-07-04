#ifndef _CELL_TASK_SERVER
#define _CELL_TASK_SERVER

#include <thread>
#include <mutex>
#include <list>
#include <functional>

#include "CELLTask.h"

class CellTaskServer
{
public:
	CellTaskServer();
	virtual ~CellTaskServer();

private:
	std::list<CellTask*> _tasks;
	std::list<CellTask*> _tasksBuf;
	std::mutex _mutex;

protected:
	void OnRun();

public:
	void AddTask(CellTask* task);
	void Start();

};

#endif // !_CELL_TASK_SERVER
