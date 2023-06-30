#include "ClientSocket.h"

ClientSocket::ClientSocket(SOCKET sockfd)
{
	_sockfd = sockfd;
	memset(_szMsgBuf, 0, sizeof(_szMsgBuf));
	_lastPos = 0;
}

SOCKET ClientSocket::sockfd()
{
	return _sockfd;
}

char* ClientSocket::msgBuf()
{
	return _szMsgBuf;
}

int ClientSocket::getLastPos()
{
	return _lastPos;
}
void ClientSocket::setLastPos(int pos)
{
	_lastPos = pos;
}

int ClientSocket::SendData(DataHeader* header)
{
	if (header)
	{
		return send(_sockfd, (const char*)header, header->len, 0);
	}
	return SOCKET_ERROR;
}
