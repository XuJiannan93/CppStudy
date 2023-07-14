#include "MyServer.h"

void MyServer::OnNetJoin(CELLClientPtr& pClient)
{
	EasyTcpServer::OnNetJoin(pClient);
	//CELLLog::Info("client<%d> join\n", (int)pClient->sockfd());
}

void MyServer::OnNetLeave(CELLClientPtr& pClient)
{
	EasyTcpServer::OnNetLeave(pClient);
	//CELLLog::Info("client<%d> leave\n", (int)pClient->sockfd());
}

void MyServer::OnNetMsg(CELLServer* pCELLServer, CELLClientPtr& pClient, netmsg_DataHeader* header)
{
	EasyTcpServer::OnNetMsg(pCELLServer, pClient, header);
	switch (header->cmd)
	{

	case CMD_LOGIN:
	{
		//Login* login = (Login*)header;
		//CELLLog::Info("recv<%d> cmd len :login [%s][%s][%d] \n", (int)pClient->sockfd(), login->username, login->password, header->len);

	/*	auto ret = std::make_shared<netmsg_LoginResult>();
		pCELLServer->AddSendTask(pClient, (DataHeaderPtr&)ret);*/

		pClient->ResetDTHeard();
		auto ret = std::make_shared<netmsg_s2c_Heart>();
		pCELLServer->AddSendTask(pClient, (DataHeaderPtr&)ret);
	}
	break;

	case CMD_LOGOUT:
	{
		//Logout* logout = (Logout*)header;
		//CELLLog::Info("recv<%d> cmd len :logout [%s][%d] \n", (int)_client, logout->username, header->len);

	/*	LogoutResult ret;
		pClient->SendData(&ret);*/
	}
	break;

	case CMD_C2S_HEART_BEAT:
	{
		pClient->ResetDTHeard();
		auto ret = std::make_shared<netmsg_s2c_Heart>();
		pCELLServer->AddSendTask(pClient, (DataHeaderPtr&)ret);
		break;
	}

	case CMD_ERROR:
		CELLLog::Info("Recv Undefined Message! \n");
		break;


	default:
	{
		//DataHeader errormsg = {};
		//send(_client, (char*)&errormsg, errormsg.len, 0);
	}
	break;
	}

}