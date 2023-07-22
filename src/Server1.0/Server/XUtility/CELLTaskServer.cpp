#include "CELLTaskServer.h"
#include "CELLLog.h"

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
	CELLLog_Info("CELLTaskServer[%d]::Close() 1", serverID);
	_thread.Close();
	CELLLog_Info("CELLTaskServer[%d]::Close() 2", serverID);
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

	for (auto pTask : _tasksBuf)
	{
		pTask();
	}
	_tasksBuf.clear();

	CELLLog_Info("CELLTaskServer[%d]::OnRun() exit", serverID);
}