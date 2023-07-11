#include "CELLTaskServer.h"

CELLTaskServer::CELLTaskServer()
{
}

CELLTaskServer::~CELLTaskServer()
{
}

void CELLTaskServer::Start()
{
	_thread.Start(
		nullptr,
		[this](CELLThread* pThread) {
			OnRun(pThread);
		},
		nullptr);
}

void CELLTaskServer::Close()
{
	printf("CELLTaskServer[%d]::Close() 1\n", serverID);
	_thread.Close();
	printf("CELLTaskServer[%d]::Close() 2\n", serverID);
}

void CELLTaskServer::AddTask(CELLTask task)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_tasksBuf.push_back(task);
}

void CELLTaskServer::OnRun(CELLThread* pThread)
{
	while (pThread->isRun())
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
	printf("CELLTaskServer[%d]::OnRun() exit\n", serverID);
}