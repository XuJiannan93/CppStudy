#include "MyClient.h"

void MyClient::OnNetMsg(netmsg_DataHeader* header)
{
	switch (header->cmd)
	{

	case CMD_LOGIN_RESULT:
	{
		//netmsg_LoginResult* rst = (netmsg_LoginResult*)header;
		CELLLog_Info("recv<%d> cmd[LOGIN] len[%d] ", _pClient->sockfd(), header->len);
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
