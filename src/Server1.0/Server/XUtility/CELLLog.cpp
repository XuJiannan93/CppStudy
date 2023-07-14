#include "CELLLog.h"


CELLLog::CELLLog()
{
	_taskServer.Start();
}

CELLLog::~CELLLog()
{
	_taskServer.Close();

	if (_logFile)
	{
		fclose(_logFile);
		_logFile = nullptr;
	}
}

void CELLLog::SetLogPath(const char* path, const char* mode)
{
	if (_logFile)
	{
		Info("CELLLog::SetLogPath() _logFile != nullptr\n");
		fclose(_logFile);
		_logFile = nullptr;
	}

	_logFile = fopen(path, mode);
	if (_logFile)
	{
		Info("CELLLog::SetLogPath(),<%s,%s>\n", path, mode);
	}
	else
	{
		Info("CELLLog::SetLogPath() failed!\n");
	}
}

void CELLLog::Info(const char* pStr)
{
	CELLLog* pLog = &Instance();
	pLog->_taskServer.AddTask([pLog, pStr]() {
		if (pLog->_logFile)
		{
			time_t t = time(nullptr);
			std::tm* now = localtime(&t);
			fprintf(pLog->_logFile, "[%d-%d-%d %d:%d:%d]:", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
				now->tm_hour, now->tm_min, now->tm_sec);
			fprintf(pLog->_logFile, "[%s]", "INFO");
			fprintf(pLog->_logFile, "%s", pStr);
			fflush(pLog->_logFile);
		}
		});

	printf("%s", pStr);
}


