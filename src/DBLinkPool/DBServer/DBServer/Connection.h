#pragma once
#include <mysql.h>
#include <string>
#include <ctime>

class Connection
{
private:
	MYSQL* _conn;
	std::clock_t _alivetime;

public:
	Connection();
	~Connection();

	bool connect(std::string ip, unsigned short port, std::string user, std::string password, std::string dbname);
	bool update(std::string sql);
	MYSQL_RES* query(std::string sql);
	void refreshAliveTime() { _alivetime = clock(); };
	std::clock_t getAliveTime() { return clock() - _alivetime; }
};
