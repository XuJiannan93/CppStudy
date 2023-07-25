#include "MyServer.h"
#include "CELLMsgStream.h"
#include "CELLConfig.h"
#include "CELLLog.h"

MyServer::MyServer()
{
	_bSendBack = CELLConfig::Instance().HasKey("-sendback");
	_bSendFull = CELLConfig::Instance().HasKey("-sendfull");
	_bCheckMsgID = CELLConfig::Instance().HasKey("-checkMsgID");
}

void MyServer::OnNetJoin(CELLClientPtr& pClient)
{
	EasyTcpServer::OnNetJoin(pClient);
	//CELLLog_Info("client<%d> join", (int)pClient->sockfd());
}

void MyServer::OnNetLeave(CELLClientPtr& pClient)
{
	EasyTcpServer::OnNetLeave(pClient);
	//CELLLog_Info("client<%d> leave", (int)pClient->sockfd());
}

void MyServer::OnNetMsg(CELLServer* pCELLServer, CELLClientPtr& pClient, netmsg_DataHeader* header)
{
	EasyTcpServer::OnNetMsg(pCELLServer, pClient, header);
	switch (header->cmd)
	{

	case CMD_LOGIN:
	{
		//Login* login = (Login*)header;
		//CELLLog_Info("recv<%d> cmd len :login [%s][%s][%d] ", (int)pClient->sockfd(), login->username, login->password, header->len);

	/*	auto ret = std::make_shared<netmsg_LoginResult>();
		pCELLServer->AddSendTask(pClient, (DataHeaderPtr&)ret);*/

		//pClient->ResetDTHeard();
		//auto ret = std::make_shared<netmsg_s2c_Heart>();
		//pCELLServer->AddSendTask(pClient, (DataHeaderPtr&)ret);

		pClient->ResetDTHeard();
		if (_bCheckMsgID)
		{
			if (header->msgID != pClient->nRecvMsgID)
			{
				CELLLog_Error("MyServer::OnNetMsg() recv msg id<%d>, expect msg id<%d>",
					header->msgID, pClient->nRecvMsgID);
			}
			pClient->nRecvMsgID++;
		}

		if (_bSendBack)
		{
			auto ret = std::make_shared<netmsg_LoginResult>();
			ret->msgID = pClient->nSendMsgID;
			if (pClient->SendData(ret) == SOCKET_ERROR)
			{
				if (_bSendFull)
				{
					//发送缓冲区满了，消息没发出去，目前直接抛弃了
					//客户端消息太多，需要考虑应对策略
					//正常连接，业务客户端不会有这么多消息
					//模拟并发测试时是否发送频率过高
					CELLLog_Warring("<socket=%d> Send Full", pClient->sockfd());
				}
			}
			else
			{
				pClient->nSendMsgID++;
			}
		}
	}
	break;

	case CMD_LOGOUT:
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
		//////////////////////////////////////////
		CELLWriteStream s;
		s.SetNetCMD(CMD_LOGOUT_RESULT);

		s.WriteInt8(n1);
		s.WriteInt16(n2);
		s.WriteInt32(n3);
		s.WriteDouble(n4);
		s.WriteFloat(n5);

		s.WriteArrayChar(arr1, len1);
		s.WriteArrayChar(arr2, len2);
		s.WriteArrayInt32(arr3, len3);
		s.Finish();
		pClient->SendData(s.data(), s.length());
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
		CELLLog::Error("Recv Undefined Message! ");
		break;


	default:
	{
		//DataHeader errormsg = {};
		//send(_client, (char*)&errormsg, errormsg.len, 0);
	}
	break;
	}

}