#include "MyClient.h"
#include "CELLMsgStream.h"

void MyClient::OnNetMsg(netmsg_DataHeader* header)
{
	switch (header->cmd)
	{

	case CMD_LOGIN_RESULT:
	{

		//netmsg_LoginResult* rst = (netmsg_LoginResult*)header;
		//CELLLog_Info("recv<%d> cmd[LOGIN] len[%d] ", _pClient->sockfd(), header->len);
	}
	break;

	case CMD_LOGOUT_RESULT:
	{
		CELLReadStream r(header);
	/*	r.ReadUint16();
		r.GetNetCMD();*/
		auto n1 = r.ReadInt8();
		auto n2 = r.ReadInt16();
		auto n3 = r.ReadInt32();
		auto n4 = r.ReadDouble();
		auto n5 = r.ReadFloat();

		char arr1[32] = {};
		auto len1 = r.ReadArrayChar(arr1, 32);
		char arr2[32] = {};
		auto len2 = r.ReadArrayChar(arr2, 32);
		int arr3[10] = {};
		auto len3 = r.ReadArrayInt32(arr3, 10);

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
