#ifndef MESSAGE_HEADER_HPP
#define MESSAGE_HEADER_HPP

#include <memory>

enum CMD
{
	CMD_ERROR,

	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_NEW_USER_JOIN,
	CMD_C2S_HEART_BEAT,
	CMD_S2C_HEART_BEAT,
};

struct netmsg_DataHeader
{
	short len;
	short cmd;

	netmsg_DataHeader()
	{
		len = sizeof(netmsg_DataHeader);
		cmd = CMD_ERROR;
	}
};

struct netmsg_Login : public netmsg_DataHeader
{
	netmsg_Login()
	{
		len = sizeof(netmsg_Login);
		cmd = CMD_LOGIN;
	}

	char username[32];
	char password[32];
	char data[32];
};

struct netmsg_LoginResult : public netmsg_DataHeader
{
	netmsg_LoginResult()
	{
		len = sizeof(netmsg_LoginResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;
	}
	int result;
	char data[92];
};

struct netmsg_Logout : public netmsg_DataHeader
{
	netmsg_Logout()
	{
		len = sizeof(netmsg_Logout);
		cmd = CMD_LOGOUT;
	}
	char username[32];
};

struct netmsg_LogoutResult : public netmsg_DataHeader
{
	netmsg_LogoutResult()
	{
		len = sizeof(netmsg_LogoutResult);
		cmd = CMD_LOGOUT_RESULT;
		result = 0;
	}
	int result;
};

struct netmsg_NewUserJoin : public netmsg_DataHeader
{
	netmsg_NewUserJoin()
	{
		len = sizeof(netmsg_NewUserJoin);
		cmd = CMD_NEW_USER_JOIN;
		sock = 0;
	}

	int sock;
};

struct netmsg_c2s_Heart : public netmsg_DataHeader
{
	netmsg_c2s_Heart()
	{
		len = sizeof(netmsg_c2s_Heart);
		cmd = CMD_C2S_HEART_BEAT;
	}
};

struct netmsg_s2c_Heart : public netmsg_DataHeader
{
	netmsg_s2c_Heart()
	{
		len = sizeof(netmsg_s2c_Heart);
		cmd = CMD_S2C_HEART_BEAT;
	}
};

typedef std::shared_ptr<netmsg_DataHeader> DataHeaderPtr;
typedef std::shared_ptr<netmsg_LoginResult> LoginResultPtr;

#endif // !MESSAGE_HEADER_HPP
