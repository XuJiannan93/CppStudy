#ifndef _CELL_LOG_H_
#define _CELL_LOG_H_

#include <stdio.h>
#include <ctime>

#include "CELLTaskServer.h"

class _X_UTILITY_API_ CELLLog
{
public:
	static CELLLog& Instance()
	{
		static CELLLog sLog;
		return sLog;
	}

	static void Info(const char* pStr);

	template<typename ...Args>
	static void Info(const char* format, Args ...args);

	void SetLogPath(const char* path, const char* mode);

private:
	CELLLog();
	~CELLLog();

private:
	FILE* _logFile = nullptr;
	CELLTaskServer _taskServer;
};

template<typename ...Args>
inline void CELLLog::Info(const char* format, Args ...args)
{
	CELLLog* pLog = &Instance();
	pLog->_taskServer.AddTask([pLog, format, args...]() {
		if (pLog->_logFile)
		{
			time_t t = time(nullptr);
			std::tm* now = localtime(&t);
			fprintf(pLog->_logFile, "[%d-%d-%d %d:%d:%d]:", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
				now->tm_hour, now->tm_min, now->tm_sec);
			fprintf(pLog->_logFile, "[%s]", "INFO");
			fprintf(pLog->_logFile, format, args...);
			fflush(pLog->_logFile);
		}
		});

	printf(format, args...);
}

#endif // !_CELL_LOG_H_

