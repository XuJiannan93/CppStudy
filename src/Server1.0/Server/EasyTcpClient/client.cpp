#include <atomic>
#include <iostream>
#include <io.h>
#include <direct.h>
#include <vector>

#include "CELLTimestamp.h"
#include "CELLTime.h"
#include "MyClient.h"
#include "CELLConfig.h"

bool g_bRun = true;
std::atomic<int> sum = 0;
std::atomic<int> sendCount = 0;
std::atomic<int> readyCount = 0;

const char* strIP = "127.0.0.1";
uint16_t nPort = 4567;
int nThread = 1;
int nClient = 1;
int nMsg = 1;
int nSendSleep = 1;
int nWorkSleep = 1;
int nSendBufSize = SEND_BUFF_SIZE;
int nRecvBufSize = RECV_BUFF_SIZE;

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

void WorkThread(CELLThread* pThread, int id)
{
	CELLLog_Info("thread<%d> start", id);

	std::vector<MyClient*> clients(nClient);

	int c = (nClient / nThread);
	int begin = (id - 1) * c;
	int end = id * c;

	for (int i = begin; i < end; i++)
	{
		if (!pThread->isRun())
			break;

		clients[i] = new MyClient(i);
		CELLThread::Sleep(0);
	}

	for (int i = begin; i < end; i++)
	{
		if (!pThread->isRun())
			break;

		if (clients[i]->InitSocket(nSendBufSize, nRecvBufSize) == INVALID_SOCKET)
			break;

		clients[i]->Connect(strIP, nPort);
		sum++;
		CELLThread::Sleep(0);
	}

	CELLLog_Info("thread<%d>, connect<begin=%d,end=%d, sum=%d>", id, begin, end, sum.load());
	readyCount++;
	while (readyCount < nThread && pThread->isRun())
	{
		CELLThread::Sleep(10);
	}

	std::shared_ptr<netmsg_Login> msg;

	msg = std::make_shared<netmsg_Login>();

	strcpy(msg->username, "xjn");
	strcpy(msg->password, "123456");

	auto t2 = CELLTime::GetNowInMilliSec();
	auto t0 = t2;
	auto dt = t0;
	CELLTimestamp tTime;
	while (pThread->isRun())
	{
		t0 = CELLTime::GetNowInMilliSec();
		dt = t0 - t2;
		t2 = t0;

		for (int i = 0; i < nMsg; i++)
		{
			for (int j = begin; j < end; j++)
			{
				if (clients[j]->IsRun() == false)
					continue;

				if (clients[j]->SendTest(msg.get()) <= 0)
					continue;

				sendCount++;
			}
		}

		for (int i = begin; i < end; i++)
		{
			if (clients[i]->IsRun() == false)
				continue;

			if (clients[i]->OnRun(0) == false)
			{
				sum--;
				continue;
			}
			clients[i]->CheckSend(dt);
		}

		CELLThread::Sleep(nWorkSleep);
	}

	for (int i = begin; i < end; i++)
	{
		clients[i]->Close();
		delete client[i];
	}

	CELLLog_Info("thread<%d> exit", id);
	readyCount--;
}

int main(int argc, char* args[])
{
	CELLConfig::Instance().Init(argc, args);

	std::string path = CELLConfig::Instance().GetExePath();
	std::string exe = ".exe";
	if (path.find(exe) != std::string::npos)
		path.replace(path.find(exe), exe.length(), "");
	std::string fnd = "EasyTcpClient";
	path.replace(path.find(fnd), fnd.length(), "..\\..\\log\\");
	const char* logPath = CELLConfig::Instance().GetString("logPath", path.data());

	if (_access(logPath, 0) == -1)
		_mkdir(logPath);

	CELLLog::Instance().SetLogPath(logPath);
	CELLLog::Instance().Init("\\ClientLog", "w");

	strIP = CELLConfig::Instance().GetString("strIP", "127.0.0.1");
	nPort = CELLConfig::Instance().GetInt("nPort", 4567);
	nThread = CELLConfig::Instance().GetInt("nThread", 4);
	nClient = CELLConfig::Instance().GetInt("nClient", 4000);
	nMsg = CELLConfig::Instance().GetInt("nMsg", 100);
	nSendSleep = CELLConfig::Instance().GetInt("nSendSleep", 1000);
	nSendBufSize = CELLConfig::Instance().GetInt("nSendBufSize", 81920);
	nRecvBufSize = CELLConfig::Instance().GetInt("nRecvBufSize", 81920);

	//std::thread t1(cmdThread);
	//t1.detach();

	CELLThread tCmd;
	tCmd.Start(nullptr, [](CELLThread* pThread) {
		while (true)
		{
			char cmdBuf[256] = {};
			scanf("%s", cmdBuf);
			if (strcmp(cmdBuf, "exit"))
			{
				pThread->Exit();
				CELLLog_Info("exit cmd thread.");
				break;
			}
			else
			{
				CELLLog_Info("unsupported command.");
			}
		}
		});

	CELLLog_Info("Client Count[%d], Thread Count[%d]", nClient, nThread);

	std::vector<CELLThread*> threads;
	for (int i = 0; i < nThread; i++)
	{
		CELLThread* t = new CELLThread();
		t->Start(nullptr, [i](CELLThread* pThread) {
			WorkThread(pThread, i + 1);
			});
		threads.push_back(t);
	}

	CELLTimestamp tTime;

	while (tCmd.isRun())
	{
		auto t = tTime.GetElapsedSecond();
		if (t >= 1.0)
		{
			CELLLog_Info("thread<%d>,clients<%d>,time<%lf>,send<%d>", nThread, nClient, t, sendCount.load());
			sendCount = 0;
			tTime.Update();
		}
	}

	for (auto t : threads)
	{
		t->Close();
		delete t;
	}

	CELLLog_Info("client exit.");

	return 0;
}