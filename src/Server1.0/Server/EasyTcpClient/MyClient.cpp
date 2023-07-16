#include "MyClient.h"

void MyClient::OnNetMsg(netmsg_DataHeader* header)
{
	switch (header->cmd)
	{

	case CMD_LOGIN_RESULT:
	{
		//netmsg_LoginResult* rst = (netmsg_LoginResult*)header;
		CELLLog::Info("recv<%d> cmd[LOGIN] len[%d] \n", _pClient->sockfd(), header->len);
	}
	break;

	case CMD_LOGOUT_RESULT:
	{
		//netmsg_LogoutResult* rst = (netmsg_LogoutResult*)header;
		CELLLog::Info("recv<%d> cmd[LOGOUT] len[%d] \n", _pClient->sockfd(), header->len);
	}
	break;

	case CMD_NEW_USER_JOIN:
	{
		//netmsg_NewUserJoin* join = (netmsg_NewUserJoin*)header;
		CELLLog::Info("recv<%d> cmd[NEW_USER_JOIN] len[%d] \n", _pClient->sockfd(), header->len);
	}
	break;

	case CMD_S2C_HEART_BEAT:
		break;

	case CMD_ERROR:
		CELLLog::Info("recv undefine message! \n");

	default:
	{
		/*	header->cmd = CMD_ERROR;
			header->len = 0;
			send(_sock, (const char*)header, sizeof(DataHeader), 0);*/

		CELLLog::Info("recv undefine message.\n");
	}
	break;
	}

}
