#ifndef _CELL_TASK_SERVER_
#define _CELL_TASK_SERVER_

#include <thread>
#include <mutex>
#include <list>
#include <functional>

#include "CELLTask.h"

typedef std::shared_ptr<CellTask> CellTaskPtr;

class CellTaskServer
{
public:
	CellTaskServer();
	virtual ~CellTaskServer();

private:
	std::list<CellTaskPtr> _tasks;
	std::list<CellTaskPtr> _tasksBuf;
	std::mutex _mutex;

protected:
	void OnRun();

public:
	void AddTask(CellTaskPtr& task);
	void Start();

};

#endif // !_CELL_TASK_SERVER_
