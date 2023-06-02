#include "EasyTcpClient.h"

void cmdThread(EasyTcpClient* client)
{
	char cmdbuf[128] = {};
	while (true)
	{

		scanf("%s", cmdbuf);
		if (strcmp(cmdbuf, "exit") == 0)
		{
			client->Close();
			printf("exit the thread.\n");
			break;
		}
		else if (strcmp(cmdbuf, "login") == 0)
		{
			Login login;
			strcpy(login.username, "xjn");
			strcpy(login.password, "123456");
			client->SendData(&login);
		}
		else if (strcmp(cmdbuf, "logout") == 0)
		{
			Logout logout;
			strcpy(logout.username, "xjn");
			client->SendData(&logout);
		}
		else
		{
			printf("???\n");
		}

	}
}

int main()
{
	EasyTcpClient* client = new EasyTcpClient();
	client->InitSocket();
	client->Connect("127.0.0.1", 4567);

	std::thread t1(cmdThread, client);
	t1.detach();

	while (client->IsRun())
	{
		client->OnRun();
	}

	client->Close();
	delete client;

	printf("client exit.\n");
	getchar();

	return 0;
}