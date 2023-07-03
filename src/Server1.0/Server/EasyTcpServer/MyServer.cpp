#include "MyServer.h"

void MyServer::OnNetJoin(ClientSocket* pClient)
{
	EasyTcpServer::OnNetJoin(pClient);
	//printf("client<%d> join\n", (int)pClient->sockfd());
}

void MyServer::OnNetLeave(ClientSocket* pClient)
{
	EasyTcpServer::OnNetLeave(pClient);
	//printf("client<%d> leave\n", (int)pClient->sockfd());
}

void MyServer::OnNetMsg(ClientSocket* pClient, DataHeader* header)
{
	EasyTcpServer::OnNetMsg(pClient, header);
	switch (header->cmd)
	{

	case CMD_LOGIN:
	{
		Login* login = (Login*)header;
		//printf("recv<%d> cmd len :login [%s][%s][%d] \n", (int)_client, login->username, login->password, header->len);

	/*	LoginResult ret;
		pClient->SendData(&ret);*/
	}
	break;

	case CMD_LOGOUT:
	{
		Logout* logout = (Logout*)header;
		//printf("recv<%d> cmd len :logout [%s][%d] \n", (int)_client, logout->username, header->len);

		LogoutResult ret;
		pClient->SendData(&ret);
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