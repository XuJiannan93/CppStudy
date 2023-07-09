#include "CellS2CTask.h"

CellS2CTask::CellS2CTask(ClientSocketPtr& pClient, DataHeaderPtr& header)
{
	_pClient = pClient;
	_pHeader = header;
}

CellS2CTask::~CellS2CTask()
{
}

void CellS2CTask::DoTask()
{
	_pClient->SendData(_pHeader);
}
