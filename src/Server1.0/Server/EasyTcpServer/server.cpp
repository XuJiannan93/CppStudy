#include <iostream>
#include <io.h>
#include <direct.h>

#include "XPrintf.h"
#include "Alloctor.hpp"
#include "MyServer.h"
#include "CELLLog.h"
#include "CELLConfig.h"

bool g_bRun = true;

void cmdThread()
{
	while (true)
	{
		char cmdbuf[256] = {};
		scanf("%s", cmdbuf);
		if (strcmp(cmdbuf, "exit") == 0)
		{
			g_bRun = false;
			CELLLog_Info("exit the thread.");
			break;
		}
		else
		{
			CELLLog_Info("???");
		}
	}
}

//const char* _ArgToStr(int argc, char* args[], int idx, const char* def, const char* key)
//{
//	if (idx >= argc)
//		CELLLog_Error("ArgToStr[%s]|idx=%d|argc=%d", key, idx, argc);
//	else
//		def = args[idx];
//	CELLLog_Info("Set Default : %s=%s", key, def);
//
//	return def;
//}
//
//int _ArgToInt(int argc, char* args[], int idx, int def, const char* key)
//{
//	if (idx >= argc)
//		CELLLog_Error("ArgToStr[%s]|idx=%d|argc=%d", key, idx, argc);
//	else
//		def = atoi(args[idx]);
//	CELLLog_Info("Set Default : %s=%d", key, def);
//
//	return def;
//}

int main(int argc, char* args[])
{
	CELLConfig::Instance().Init(argc, args);

	//const char* strIP = _ArgToStr(argc, args, 1, "any", "IP");
	//uint16_t nPort = _ArgToInt(argc, args, 2, 4567, "Port");
	//int nThread = _ArgToInt(argc, args, 3, 1, "ThreadCount");
	//int nClient = _ArgToInt(argc, args, 4, 1, "ClientCount");

	const char* strIP = CELLConfig::Instance().GetString("strIP", "any");
	uint16_t nPort = CELLConfig::Instance().GetInt("nPort", 4567);
	uint16_t nThread = CELLConfig::Instance().GetInt("nThread", 4);

	std::string path = CELLConfig::Instance().GetExePath();
	std::string exe = ".exe";
	if (path.find(exe) != std::string::npos)
		path.replace(path.find(exe), exe.length(), "");
	std::string fnd = "EasyTcpServer";
	path.replace(path.find(fnd), fnd.length(), "..\\..\\log\\");
	//const char* logPath = _ArgToStr(argc, args, 5, path.data(), "LogPath");
	const char* logPath = CELLConfig::Instance().GetString("logPath", path.data());

	if (strcmp(strIP, "any") == 0)
		strIP = nullptr;

	if (_access(logPath, 0) == -1)
		_mkdir(logPath);

	CELLLog::Instance().SetLogPath(logPath);
	CELLLog::Instance().Init("ServerLog", "w");

	auto server = std::make_shared<MyServer>();
	server->InitSocket();
	server->Bind(strIP, nPort);
	server->Listen(64);
	server->Start(nThread);

	//std::thread t(cmdThread);
	//t.detach();
	while (true)
	{
		char cmdbuf[256] = {};
		scanf("%s", cmdbuf);
		if (strcmp(cmdbuf, "exit") == 0)
		{
			server->Close();
			break;
		}
		else
		{
			CELLLog_Info("???");
		}
	}
	CELLLog_Info("exit");

	return 0;
}

