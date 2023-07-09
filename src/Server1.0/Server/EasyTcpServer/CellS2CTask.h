#ifndef _CELL_SEND_MSG_TO_CLIENT_TASK_H
#define _CELL_SEND_MSG_TO_CLIENT_TASK_H

#include "CELLTask.h"
#include "ClientSocket.h"

class CellS2CTask : public CellTask
{
public:
	CellS2CTask(ClientSocketPtr& pClient, DataHeaderPtr& header);
	~CellS2CTask();

private:
	ClientSocketPtr _pClient;
	DataHeaderPtr _pHeader;

public:
	void DoTask() override;
};

#endif // !_CELL_SEND_MSG_TO_CLIENT_TASK_H
