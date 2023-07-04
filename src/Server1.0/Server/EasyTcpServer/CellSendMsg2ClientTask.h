#ifndef _CELL_SEND_MSG_TO_CLIENT_TASK_H
#define _CELL_SEND_MSG_TO_CLIENT_TASK_H

#include "CELLTask.h"
#include "ClientSocket.h"

class CellSendMsg2ClientTask : public CellTask
{
public:
	CellSendMsg2ClientTask(ClientSocket* pClient, DataHeader* header);
	~CellSendMsg2ClientTask();

private:
	ClientSocket* _pClient;
	DataHeader* _pHeader;

public:
	void DoTask() override;
};

#endif // !_CELL_SEND_MSG_TO_CLIENT_TASK_H
