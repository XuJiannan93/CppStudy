#ifndef _CELL_CLIENT_H_
#define _CELL_CLIENT_H_

#include <stdio.h>
#include <vector>
#include <memory>

#include "CELL.hpp"
#include "MessageHeader.hpp"
#include "ObjectPoolBase.hpp"

#define CLIENT_HEART_DEAD_TIME 60000
#define CLIENT_SEND_BUFFF_TIME 200

class CELLClient : public ObjectPoolBase<CELLClient, 10000>
{
private:
	SOCKET _sockfd;
	char _szMsgBuf[RECV_BUFF_SIZE] = {};
	char _szSendBuf[SEND_BUFF_SIZE] = {};
	int _lastPos = 0;
	int _lastSendPos = 0;
	time_t _dtHeart;
	time_t _dtSend;

private:
	void _ResetDTSend();
	int _SendDataImmediately();

public:
	SOCKET sockfd();
	char* msgBuf();
	int getLastPos();
	void setLastPos(int pos);
	int SendData(const DataHeaderPtr& header);
	void ResetDTHeard();
	bool CheckHeart(time_t dt);
	bool CheckSend(time_t dt);

	CELLClient(SOCKET sockfd = INVALID_SOCKET);
	~CELLClient();
};

#ifndef _SHARED_PTR_CLIENT_SOCKET_
#define _SHARED_PTR_CLIENT_SOCKET_
typedef std::shared_ptr<CELLClient> ClientSocketPtr;
#endif // !_SHARED_PTR_CLIENT_SOCKET_

#endif // !_CELL_CLIENT_H_
