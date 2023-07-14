//#include "EasyTcpServer.h"
#include "Alloctor.h"
#include "MyServer.h"
#include "CELLLog.h"

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
			CELLLog::Info("exit the thread.\n");
			break;
		}
		else
		{
			CELLLog::Info("???\n");
		}
	}
}

int main()
{
	CELLLog::Instance().SetLogPath("../../bin/log/ServerLog.txt", "w");

	auto server = std::make_shared<MyServer>();
	server->InitSocket();
	server->Bind(nullptr, 4567);
	server->Listen(5);
	server->Start(4);

	/*std::thread t(cmdThread);
	t.detach();*/
	while (true)
	{
		char cmdbuf[256] = {};
		scanf("%s", cmdbuf);
		if (strcmp(cmdbuf, "exit") == 0)
		{
			server->Close();
			CELLLog::Info("exit the thread.\n");
			break;
		}
		else
		{
			CELLLog::Info("???\n");
		}
	}

	return 0;
}