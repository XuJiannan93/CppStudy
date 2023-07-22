#include "CELLLog.h"
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

void CELLLog::SetLogPath(const char* logPath)
{
	_fileName = logPath;
}

void CELLLog::Init(const char* logName, const char* mode)
{
	if (_logFile)
	{
		Info("CELLLog::SetLogPath() _logFile != nullptr");
		fclose(_logFile);
		_logFile = nullptr;
	}

	time_t t = time(nullptr);
	std::tm* now = localtime(&t);
	sprintf(_logPath, "%s%s[%d-%02d-%02d_%02d-%02d-%02d].txt", _fileName, logName,
		now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
		now->tm_hour, now->tm_min, now->tm_sec);

	_logFile = fopen(_logPath, mode);
	if (_logFile)
	{
		Info("CELLLog::SetLogPath(),<%s,%s>", _logPath, mode);
	}
	else
	{
		Info("CELLLog::SetLogPath(), <%s,%s> failed!", _logPath, mode);
	}
}

void CELLLog::Info(const char* pStr)
{
	Info("%s", pStr);
}

void CELLLog::Debug(const char* pStr)
{
	Debug("%s", pStr);
}

void CELLLog::Warring(const char* pStr)
{
	Warring("%s", pStr);
}

void CELLLog::Error(const char* pStr)
{
	Error("%s", pStr);
}


