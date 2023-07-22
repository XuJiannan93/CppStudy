#ifndef _CELL_LOG_H_
#define _CELL_LOG_H_

#include <stdio.h>
#include <ctime>

#include "CELLTaskServer.h"

#if _DEBUG
#define CELLLog_Debug(...) CELLLog::Debug(__VA_ARGS__)
#else
#define CELLLog_Debug(...)
#endif

#if _WARRING
#define CELLLog_Warring(...) CELLLog::Warring(__VA_ARGS__)
#else
#define CELLLog_Warring(...)
#endif

#if _INFO 
#define CELLLog_Info(...) CELLLog::Info(__VA_ARGS__)
#else
#define CELLLog_Info(...)
#endif

#define CELLLog_Error(...) CELLLog::Error(__VA_ARGS__)

class _X_UTILITY_API_ CELLLog
{
public:
	static CELLLog& Instance()
	{
		static CELLLog sLog;
		return sLog;
	}

	static void Info(const char* pStr);
	static void Debug(const char* pStr);
	static void Warring(const char* pStr);
	static void Error(const char* pStr);


	template<typename ...Args>
	static void Info(const char* format, Args ...args);

	template<typename ...Args>
	static void Debug(const char* format, Args ...args);

	template<typename ...Args>
	static void Warring(const char* format, Args ...args);

	template<typename ...Args>
	static void Error(const char* format, Args ...args);

	void SetLogPath(const char* path);
	void Init(const char* path, const char* mode);

private:
	CELLLog();
	~CELLLog();

	template<typename ...Args>
	static void Echo(const char* type, const char* format, Args ...args);

private:
	FILE* _logFile = nullptr;
	CELLTaskServer _taskServer;
	char _logPath[256] = {};
	const char* _fileName;
};

template<typename ...Args>
inline void CELLLog::Echo(const char* type, const char* format, Args ...args)
{
	CELLLog* pLog = &Instance();
	pLog->_taskServer.AddTask([pLog, type, format, args...]() {
		if (pLog->_logFile)
		{
			time_t t = time(nullptr);
			std::tm* now = localtime(&t);
			fprintf(pLog->_logFile, "[%d-%02d-%02d %02d:%02d:%02d]:", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
				now->tm_hour, now->tm_min, now->tm_sec);
			fprintf(pLog->_logFile, "[%s]", type);
			fprintf(pLog->_logFile, format, args...);
			fprintf(pLog->_logFile, "%s", "\n");
			fflush(pLog->_logFile);
		}

		printf("[%s]", type);
		printf(format, args...);
		printf("%s", "\n");
		});
}

template<typename ...Args>
inline void CELLLog::Info(const char* format, Args ...args)
{
	Echo("INFO", format, args...);
}

template<typename ...Args>
inline void CELLLog::Debug(const char* format, Args ...args)
{
	Echo("DEBUG", format, args...);
}

template<typename ...Args>
inline void CELLLog::Warring(const char* format, Args ...args)
{
	Echo("WARRING", format, args...);
}

template<typename ...Args>
inline void CELLLog::Error(const char* format, Args ...args)
{
	Echo("ERROR", format, args...);
}
#endif // !_CELL_LOG_H_

