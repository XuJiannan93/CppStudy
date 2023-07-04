#include "CellTaskServer.h"

CellTaskServer::CellTaskServer()
{
}

CellTaskServer::~CellTaskServer()
{
}

void CellTaskServer::Start()
{
	std::thread t(std::mem_fn(&CellTaskServer::OnRun), this);
	t.detach();
}

void CellTaskServer::AddTask(CellTask* task)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_tasksBuf.push_back(task);
}

void CellTaskServer::OnRun()
{
	while (true)
	{
		if (!_tasksBuf.empty())
		{
			std::lock_guard<std::mutex> lock(_mutex);
			for (auto pTask : _tasksBuf)
			{
				_tasks.push_back(pTask);
			}
			_tasksBuf.clear();
		}

		if (_tasks.empty())
		{
			std::chrono::milliseconds t(1);
			std::this_thread::sleep_for(t);

			continue;
		}

		for (auto pTask : _tasks)
		{
			pTask->DoTask();
			delete pTask;
		}
		_tasks.clear();
	}
}