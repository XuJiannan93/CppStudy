#pragma once

#include <string>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <memory>
#include <functional>

#include "Connection.h"


class ConnectionPool
{
private:
	std::string _ip;
	unsigned short _port;
	std::string _username;
	std::string _password;
	std::string _dbname;
	int _initSize;
	int _maxSize;
	int _maxIdleTime;
	int _connectionTimeout;

	std::queue<Connection*> _connectionQue;
	std::mutex _queueMutex;
	std::condition_variable cv;
	std::atomic_int _connectionCnt;

private:
	ConnectionPool();

	bool loadConfigFile();
	void produceConnectionTask();
	void scanConnectionTask();

public:
	static ConnectionPool* getConnectionPool();
	std::shared_ptr<Connection> getConnection();
};
