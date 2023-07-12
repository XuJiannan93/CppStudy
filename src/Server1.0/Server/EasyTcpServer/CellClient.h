#ifndef _CELL_CLIENT_H_
#define _CELL_CLIENT_H_

#include <stdio.h>
#include <vector>
#include <memory>

#include "CELL.hpp"
#include "MessageHeader.hpp"
#include "ObjectPoolBase.hpp"
#include "CELLBuffer.h"

#define CLIENT_HEART_DEAD_TIME 60000
#define CLIENT_SEND_BUFFF_TIME 200

class CELLClient : public ObjectPoolBase<CELLClient, 10000>
{
private:
	SOCKET _sockfd;
	CELLBuffer _recvBuf;
	CELLBuffer _sendBuf;
	time_t _dtHeart;
	time_t _dtSend;
	int _sendBuffFullCount = 0;

private:
	void _ResetDTSend();

public:
	SOCKET sockfd();
	int SendData(const DataHeaderPtr& header);
	int RecvData();
	int SendDataImmediately();
	void ResetDTHeard();
	bool CheckHeart(time_t dt);
	bool CheckSend(time_t dt);
	bool HasMsg();
	netmsg_DataHeader* front_msg();
	void pop_front_msg();

	CELLClient(SOCKET sockfd = INVALID_SOCKET);
	~CELLClient();
};

#ifndef _SHARED_PTR_CLIENT_SOCKET_
#define _SHARED_PTR_CLIENT_SOCKET_
typedef std::shared_ptr<CELLClient> CELLClientPtr;
#endif // !_SHARED_PTR_CLIENT_SOCKET_

#endif // !_CELL_CLIENT_H_
