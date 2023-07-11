//#include "EasyTcpServer.h"
#include "Alloctor.h"
#include "MyServer.h"

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
			printf("exit the thread.\n");
			break;
		}
		else
		{
			printf("???\n");
		}
	}
}

int main()
{
	auto server = std::make_shared<MyServer>();
	server->InitSocket();
	server->Bind(nullptr, 4567);
	server->Listen(5);
	server->Start(4);

	std::thread t(cmdThread);
	t.detach();

	while (g_bRun)
	{
		server->OnRun();
	}

	server->Close();

	//while (true)
	//{
	//	Sleep(10000);
	//}

	return 0;
}