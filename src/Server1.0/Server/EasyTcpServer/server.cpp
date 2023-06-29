#include "EasyTcpServer.h"

int main()
{
	EasyTcpServer* server = new EasyTcpServer();
	server->InitSocket();
	server->Bind(nullptr, 4567);
	server->Listen(5);
	server->Start();

	while (server->IsRun())
	{
		server->OnRun();
	}

	server->Close();
	delete server;

	getchar();

	return 0;
}