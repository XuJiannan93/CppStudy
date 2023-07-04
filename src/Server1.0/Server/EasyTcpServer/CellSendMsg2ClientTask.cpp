#include "CellSendMsg2ClientTask.h"

CellSendMsg2ClientTask::CellSendMsg2ClientTask(ClientSocket* pClient, DataHeader* header)
{
	_pClient = pClient;
	_pHeader = header;
}

CellSendMsg2ClientTask::~CellSendMsg2ClientTask()
{
}

void CellSendMsg2ClientTask::DoTask()
{
	_pClient->SendData(_pHeader);
	delete _pHeader;
}
