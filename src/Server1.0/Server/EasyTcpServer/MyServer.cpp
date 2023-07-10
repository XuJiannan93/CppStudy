#include "MyServer.h"

void MyServer::OnNetJoin(ClientSocketPtr& pClient)
{
	EasyTcpServer::OnNetJoin(pClient);
	//printf("client<%d> join\n", (int)pClient->sockfd());
}

void MyServer::OnNetLeave(ClientSocketPtr& pClient)
{
	EasyTcpServer::OnNetLeave(pClient);
	//printf("client<%d> leave\n", (int)pClient->sockfd());
}

void MyServer::OnNetMsg(CellServer* pCellServer, ClientSocketPtr& pClient, netmsg_DataHeader* header)
{
	EasyTcpServer::OnNetMsg(pCellServer, pClient, header);
	switch (header->cmd)
	{

	case CMD_LOGIN:
	{
		//Login* login = (Login*)header;
		//printf("recv<%d> cmd len :login [%s][%s][%d] \n", (int)pClient->sockfd(), login->username, login->password, header->len);

		auto ret = std::make_shared<netmsg_LoginResult>();
		pCellServer->AddSendTask(pClient, (DataHeaderPtr&)ret);
	}
	break;

	case CMD_LOGOUT:
	{
		//Logout* logout = (Logout*)header;
		//printf("recv<%d> cmd len :logout [%s][%d] \n", (int)_client, logout->username, header->len);

	/*	LogoutResult ret;
		pClient->SendData(&ret);*/
	}
	break;

	case CMD_ERROR:
		printf("Recv Undefined Message! \n");
		break;


	default:
	{
		//DataHeader errormsg = {};
		//send(_client, (char*)&errormsg, errormsg.len, 0);
	}
	break;
	}

}