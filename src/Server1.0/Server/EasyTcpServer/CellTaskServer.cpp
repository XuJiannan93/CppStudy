#include "CellTaskServer.h"

CellTaskServer::CellTaskServer()
{
}

CellTaskServer::~CellTaskServer()
{
}

void CellTaskServer::Start()
{
	_isRun = true;
	std::thread t(std::mem_fn(&CellTaskServer::OnRun), this);
	t.detach();
}

void CellTaskServer::Close()
{
	printf("CellTaskServer[%d]::Close() 1\n", serverID);
	_isRun = false;
	_sem.Wait();
	printf("CellTaskServer[%d]::Close() 2\n", serverID);
}

void CellTaskServer::AddTask(CellTask task)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_tasksBuf.push_back(task);
}

void CellTaskServer::OnRun()
{
	while (_isRun)
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
			//pTask->DoTask();
			pTask();
		}
		_tasks.clear();
	}
	_sem.Wakeup();
	printf("CellTaskServer[%d]::OnRun() exit\n", serverID);
}