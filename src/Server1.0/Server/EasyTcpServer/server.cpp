//#include "EasyTcpServer.h"
#include "Alloctor.h"
#include "MyServer.h"


int main()
{
	MyServer* server = new MyServer();
	server->InitSocket();
	server->Bind(nullptr, 4567);
	server->Listen(5);
	server->Start(4);

	while (server->IsRun())
	{
		server->OnRun();
	}

	server->Close();
	delete server;

	getchar();

	return 0;
}