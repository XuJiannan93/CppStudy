#include "EasyTcpClient.h"

bool g_bRun = true;
void cmdThread(/*EasyTcpClient* client*/)
{
	while (true)
	{
		char cmdbuf[256] = {};
		scanf("%s", cmdbuf);
		if (strcmp(cmdbuf, "exit") == 0)
		{
			//client->Close();
			g_bRun = false;
			printf("exit the thread.\n");
			break;
		}
		//else if (strcmp(cmdbuf, "login") == 0)
		//{
		//	Login login;
		//	strcpy(login.username, "xjn");
		//	strcpy(login.password, "123456");
		//	client->SendData(&login);
		//}
		//else if (strcmp(cmdbuf, "logout") == 0)
		//{
		//	Logout logout;
		//	strcpy(logout.username, "xjn");
		//	client->SendData(&logout);
		//}
		else
		{
			printf("???\n");
		}
	}
}

//客户端数量
const int cCount = 1000;
//发送线程数量
const int tCount = 4;
EasyTcpClient* client[cCount];

void sendThread(int id)
{
	int c = (cCount / tCount);
	int begin = (id - 1) * c;
	int end = id * c;

	for (int n = begin; n < end; n++)
	{
		client[n] = new EasyTcpClient();
	}

	for (int n = begin; n < end; n++)
	{
		client[n]->Connect("127.0.0.1", 4567);
		printf("Connect=%d\n", n);
	}


	Login login;
	strcpy(login.username, "xjn");
	strcpy(login.password, "123456");

	while (g_bRun)
	{
		for (int n = begin; n < end; n++)
		{
			client[n]->SendData(&login);
		}
	}

	for (int n = begin; n < end; n++)
	{
		client[n]->Close();
	}


}

int main()
{

	std::thread t1(cmdThread);
	t1.detach();

	//启动发送线程
	for (int n = 0; n < tCount; n++)
	{
		std::thread t0(sendThread, n + 1);
		t0.detach();
	}

	//EasyTcpClient* client = new EasyTcpClient();
	//client->InitSocket();
	//client->Connect("127.0.0.1", 4567);

	//std::thread t1(cmdThread, client);
	//t1.detach();

	//while (client->IsRun())
	//{
	//	client->OnRun();
	//}

	//client->Close();
	//delete client;

	while (g_bRun)
	{
		Sleep(100);
	}

	printf("client exit.\n");
	//getchar();

	return 0;
}