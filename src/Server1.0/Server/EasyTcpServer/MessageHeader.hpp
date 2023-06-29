#ifndef MESSAGE_HEADER_HPP
#define MESSAGE_HEADER_HPP

enum CMD
{
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_NEW_USER_JOIN,
	CMD_ERROR,
};

struct DataHeader
{
	short len;
	short cmd;

	DataHeader()
	{
		len = sizeof(DataHeader);
		cmd = CMD_ERROR;
	}
};

struct Login : public DataHeader
{
	Login()
	{
		len = sizeof(Login);
		cmd = CMD_LOGIN;
	}

	char username[32];
	char password[32];
	char data[32];
};

struct LoginResult : public DataHeader
{
	LoginResult()
	{
		len = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;
	}
	int result;
	char data[92];
};

struct Logout : public DataHeader
{
	Logout()
	{
		len = sizeof(Logout);
		cmd = CMD_LOGOUT;
	}
	char username[32];
};

struct LogoutResult : public DataHeader
{
	LogoutResult()
	{
		len = sizeof(LogoutResult);
		cmd = CMD_LOGOUT_RESULT;
		result = 0;
	}
	int result;
};

struct NewUserJoin : public DataHeader
{
	NewUserJoin()
	{
		len = sizeof(NewUserJoin);
		cmd = CMD_NEW_USER_JOIN;
		sock = 0;
	}

	int sock;
};

#endif // !MESSAGE_HEADER_HPP
