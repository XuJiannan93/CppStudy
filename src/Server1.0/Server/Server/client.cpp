#include <atomic>

#include "CELLTimestamp.h"
#include "CELLMsgStream.h"
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
			CELLLog_Info("exit the thread.");
			break;
		}
		else
		{
			CELLLog_Info("???");
		}
	}
}

//客户端数量
const int cCount = 20;
//发送线程数量
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

	CELLLog_Info("thread[%d] start,Connect sum [%d]", id, sum.load());

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

	CELLLog_Info("thread[%d] exit,Connect sum [%d]", id, sum.load());
}


int main()
{
	CELLLog::Instance().Init("../../bin/log/ClientLog.txt", "w");

	CELLWriteStream s;
	s.SetNetCMD(CMD_LOGOUT);

	s.WriteInt8(1);
	s.WriteInt16(5);
	s.WriteInt32(100);
	s.WriteDouble(5.55);
	s.WriteFloat(3.14f);

	const char* str = "client";
	s.WriteArrayChar(str, strlen(str));
	char a[] = "hello world";
	s.WriteArrayChar(a, strlen(a));
	int32_t b[] = { 1,2,3,4,5,6 };
	s.WriteArrayInt32(b, 6);
	s.Finish();

	//auto n1 = s.ReadInt8();
	//auto n2 = s.ReadInt16();
	//auto n3 = s.ReadInt32();
	//auto n4 = s.ReadDouble();
	//auto n5 = s.ReadFloat();

	//char arr1[32] = {};
	//auto len1 = s.ReadArrayChar(arr1, 32);
	//char arr2[32] = {};
	//auto len2 = s.ReadArrayChar(arr2, 32);
	//int arr3[10] = {};
	//auto len3 = s.ReadArrayInt32(arr3, 10);

	MyClient client;
	client.Connect("127.0.0.1", 4567);

	while (client.IsRun())
	{
		client.OnRun();
		client.SendData(s.data(), s.length());
		CELLThread::Sleep(10);
	}

	//delete[] arr1;


	return 0;
}