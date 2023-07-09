#include <atomic>

#include "EasyTcpClient.h"
#include "CELLTimestamp.h"

bool g_bRun = true;
std::atomic<int> sum = 0;
std::atomic<int> sendCount = 0;
std::atomic<int> readyCount = 0;

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

//�ͻ�������
const int cCount = 1000;
//�����߳�����
const int tCount = 8;
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
		sum++;
	}

	printf("thread[%d] start,Connect sum [%d]\n", id, sum.load());

	readyCount++;
	while (readyCount < tCount)
	{
		std::chrono::milliseconds t1(3000);
		std::this_thread::sleep_for(t1);
	}

	Login login[1];

	for (int n = 0; n < 10; n++)
	{
		strcpy(login[n].username, "xjn");
		strcpy(login[n].password, "123456");
	}

	const int nLen = sizeof(login);

	//CELLTimestamp tTime;

	while (g_bRun)
	{
		for (int n = begin; n < end; n++)
		{
			/*	if (tTime.GetElapsedSecond() > 1.0)*/
				//{
			auto ret = client[n]->SendData(login, nLen);
			if (ret != SOCKET_ERROR)
				sendCount++;

			//tTime.Update();
			//}
			client[n]->OnRun();
		}
	}

	for (int n = begin; n < end; n++)
	{
		client[n]->Close();
		delete client[n];
		sum--;
	}

	printf("thread[%d] exit,Connect sum [%d]\n", id, sum.load());


}

int main()
{

	std::thread t1(cmdThread);
	t1.detach();

	//���������߳�
	for (int n = 0; n < tCount; n++)
	{
		std::thread t0(sendThread, n + 1);
		t0.detach();
	}

	CELLTimestamp tTime;

	while (g_bRun)
	{
		auto t = tTime.GetElapsedSecond();
		if (t >= 1.0)
		{
			printf("thread<%d>,clients<%d>,time<%lf>,send<%d>\n", tCount, cCount, t, (int)(sendCount.load() / t));
			tTime.Update();
			sendCount = 0;
		}
		Sleep(1);
	}

	printf("client exit.\n");
	//getchar();

	return 0;
}