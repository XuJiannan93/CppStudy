#ifndef _CELL_SEND_MSG_TO_CLIENT_TASK_H_
#define _CELL_SEND_MSG_TO_CLIENT_TASK_H_

#include "CELLTask.h"
#include "CellClient.h"

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

#endif // !_CELL_SEND_MSG_TO_CLIENT_TASK_H_
