#include <atomic>

#include "CELLTimestamp.h"
#include "MyClient.h"

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
			CELLLog::Info("exit the thread.\n");
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
			CELLLog::Info("???\n");
		}
	}
}

//�ͻ�������
const int cCount = 20;
//�����߳�����
const int tCount = 4;
EasyTcpClient* client[cCount];

void recvThread(int begin, int end)
{
	while (g_bRun)
	{
		for (int n = begin; n < end; n++)
		{
			client[n]->OnRun();
		}
	}
}

void sendThread(int id)
{
	int c = (cCount / tCount);
	int begin = (id - 1) * c;
	int end = id * c;

	for (int n = begin; n < end; n++)
	{
		client[n] = new MyClient();
	}

	for (int n = begin; n < end; n++)
	{
		client[n]->Connect("127.0.0.1", 4567);
		sum++;
	}

	CELLLog::Info("thread[%d] start,Connect sum [%d]\n", id, sum.load());

	readyCount++;
	while (readyCount < tCount)
	{
		std::chrono::milliseconds t1(100);
		std::this_thread::sleep_for(t1);
	}

	std::thread t1(recvThread, begin, end);
	t1.detach();


	std::shared_ptr<netmsg_Login> msg;

	msg = std::make_shared<netmsg_Login>();

	strcpy(msg->username, "xjn");
	strcpy(msg->password, "123456");

	while (g_bRun)
	{
		for (int n = begin; n < end; n++)
		{
			auto ret = client[n]->SendData(msg);
			if (ret != SOCKET_ERROR)
				sendCount++;

			//client[n]->OnRun();
		}

		std::chrono::milliseconds t(100);
		std::this_thread::sleep_for(t);
	}

	for (int n = begin; n < end; n++)
	{
		client[n]->Close();
		delete client[n];
		sum--;
	}

	CELLLog::Info("thread[%d] exit,Connect sum [%d]\n", id, sum.load());


}


int main()
{
	CELLLog::Instance().SetLogPath("../../bin/log/ClientLog.txt", "w");

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
			CELLLog::Info("thread<%d>,clients<%d>,time<%lf>,send<%d>\n", tCount, cCount, t, (int)(sendCount.load() / t));
			tTime.Update();
			sendCount = 0;
		}
		Sleep(1);
	}

	CELLLog::Info("client exit.\n");
	//getchar();

	return 0;
}