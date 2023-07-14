#include "CELLNetwork.h"

CELLNetwork::CELLNetwork()
{
#ifdef _WIN32
	WORD ver = MAKEWORD(2, 2);
	WSADATA data;
	WSAStartup(ver, &data);
#endif

#ifndef _WIN32
	signal(SIGPIPE, SIG_IGN);
#endif // !_WIN32

}

CELLNetwork::~CELLNetwork()
{
#ifdef _WIN32
	WSACleanup();
#endif

}

void CELLNetwork::Init()
{
	static CELLNetwork obj;
}
