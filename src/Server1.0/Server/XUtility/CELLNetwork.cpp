#include "CELLNetwork.h"
#include "CELLLog.h"

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

int CELLNetwork::Make_ReuseAddr(SOCKET fd)
{
	int flag = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag, sizeof(flag)) == SOCKET_ERROR)
	{
		CELLLog_Warring("setsockopt socket<%d> SO_REUSEADDR failed.", (int)fd);
		return SOCKET_ERROR;
	}

	return 0;

}
