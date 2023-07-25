#include "MyClient.h"
#include "CELLConfig.h"

MyClient::MyClient(int id)
{
	_id = id;
	_bCheckMsgID = CELLConfig::Instance().HasKey("-checkMsgID");
}

void MyClient::OnNetMsg(netmsg_DataHeader* header)
{
	switch (header->cmd)
	{

	case CMD_LOGIN_RESULT:
	{
		//netmsg_LoginResult* rst = (netmsg_LoginResult*)header;
		if (_bCheckMsgID)
		{
			if (header->msgID != _nRecvMsgID)
			{
				CELLLog_Error("MyClient::OnNetMsg() recv msg id<%d>, expect msg id<%d>", header->msgID, _nRecvMsgID);
			}
			++_nRecvMsgID;
		}
		//CELLLog_Info("recv<%d> cmd[LOGIN] len[%d] ", _pClient->sockfd(), header->len);
	}
	break;

	case CMD_LOGOUT_RESULT:
	{
		//netmsg_LogoutResult* rst = (netmsg_LogoutResult*)header;
		CELLLog_Info("recv<%d> cmd[LOGOUT] len[%d] ", _pClient->sockfd(), header->len);
	}
	break;

	case CMD_NEW_USER_JOIN:
	{
		//netmsg_NewUserJoin* join = (netmsg_NewUserJoin*)header;
		CELLLog_Info("recv<%d> cmd[NEW_USER_JOIN] len[%d] ", _pClient->sockfd(), header->len);
	}
	break;

	case CMD_S2C_HEART_BEAT:
		break;

	case CMD_ERROR:
		CELLLog_Info("recv undefine message! ");

	default:
	{
		/*	header->cmd = CMD_ERROR;
			header->len = 0;
			send(_sock, (const char*)header, sizeof(DataHeader), 0);*/

		CELLLog_Info("recv undefine message.");
	}
	break;
	}
}

int MyClient::SendTest(netmsg_Login* header)
{
	if (_nSendCount == 0)
		return 0;

	auto login = std::make_shared<netmsg_Login>();
	login->msgID = _nSendMsgID;

	int ret = SendData(login);
	if (ret == SOCKET_ERROR)
		return ret;

	_nSendMsgID++;
	_nSendCount--;

	return ret;
}

bool MyClient::CheckSend(time_t dt)
{
	_tRestTime += dt;
	if (_tRestTime < nSendSleep)
		return _nSendCount > 0;

	_tRestTime -= nSendSleep;
	_nSendCount = nMsg;

	return _nSendCount > 0;
}
