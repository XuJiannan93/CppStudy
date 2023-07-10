#ifndef _CELL_CLIENT_H_
#define _CELL_CLIENT_H_

#include <stdio.h>
#include <vector>
#include <memory>

#include "Cell.hpp"
#include "MessageHeader.hpp"
#include "ObjectPoolBase.hpp"

class CellClient : public ObjectPoolBase<CellClient, 10000>
{
private:
	SOCKET _sockfd;
	char _szMsgBuf[RECV_BUFF_SIZE] = {};
	char _szSendBuf[SEND_BUFF_SIZE] = {};
	int _lastPos = 0;
	int _lastSendPos = 0;

public:
	SOCKET sockfd();
	char* msgBuf();
	int getLastPos();
	void setLastPos(int pos);
	int SendData(DataHeaderPtr& header);

	CellClient(SOCKET sockfd = INVALID_SOCKET);
	~CellClient();
};

#ifndef _SHARED_PTR_CLIENT_SOCKET_
#define _SHARED_PTR_CLIENT_SOCKET_
typedef std::shared_ptr<CellClient> ClientSocketPtr;
#endif // !_SHARED_PTR_CLIENT_SOCKET_

#endif // !_CELL_CLIENT_H_