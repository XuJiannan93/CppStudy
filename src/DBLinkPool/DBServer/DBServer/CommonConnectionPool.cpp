#include <iostream>
#include <string>

#include "public.h"
#include "CommonConnectionPool.h"

ConnectionPool* ConnectionPool::getConnectionPool()
{
	static ConnectionPool pool;
	return &pool;
}

bool ConnectionPool::loadConfigFile()
{
	FILE* pf = fopen("mysql.ini", "r");
	if (pf == nullptr)
	{
		LOG("mysql.ini file is not exist.");
		return false;
	}

	while (!feof(pf))
	{
		char line[1024] = { 0 };
		fgets(line, 1024, pf);
		std::string str = line;
		int idx = str.find('=', 0);
		if (idx == -1)
			continue;

		int endidx = str.find('\n', idx);
		std::string key = str.substr(0, idx);
		std::string value = str.substr(idx + 1, endidx - idx - 1);

		if (key == "ip")
			_ip = value;
		else if (key == "port")
			_port = atoi(value.c_str());
		else if (key == "username")
			_username = value;
		else if (key == "password")
			_password = value;
		else if (key == "dbname")
			_dbname = value;
		else if (key == "initSize")
			_initSize = atoi(value.c_str());
		else if (key == "maxSize")
			_maxSize = atoi(value.c_str());
		else if (key == "maxIdleTime")
			_maxIdleTime = atoi(value.c_str());
		else if (key == "connectionTimeout")
			_connectionTimeout = atoi(value.c_str());
	}

	return true;
}

ConnectionPool::ConnectionPool()
{
	if (!loadConfigFile())
		return;

	for (int i = 0; i < _initSize; ++i)
	{
		Connection* p = new Connection();
		p->connect(_ip, _port, _username, _password, _dbname);
		p->refreshAliveTime();
		_connectionQue.push(p);
		_connectionCnt++;
	}

	std::thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));
	produce.detach();

	std::thread scan(std::bind(&ConnectionPool::scanConnectionTask, this));
	scan.detach();
}

void ConnectionPool::produceConnectionTask()
{
	for (;;)
	{
		std::unique_lock<std::mutex> lock(_queueMutex);
		while (_connectionQue.empty() == false)
		{
			cv.wait(lock);
		}

		if (_connectionCnt < _maxSize)
		{
			Connection* p = new Connection();
			p->connect(_ip, _port, _username, _password, _dbname);
			p->refreshAliveTime();
			_connectionQue.push(p);
			_connectionCnt++;
		}

		cv.notify_all();
	}
}

std::shared_ptr<Connection> ConnectionPool::getConnection()
{
	std::unique_lock<std::mutex> lock(_queueMutex);
	while (_connectionQue.empty())
	{
		if (cv.wait_for(lock, std::chrono::milliseconds(_connectionTimeout)) == std::cv_status::timeout)
		{
			if (_connectionQue.empty())
			{
				LOG("get empty connection out of time.. get connection failed.");
				return nullptr;
			}
		}
	}

	std::shared_ptr<Connection> sp(_connectionQue.front(),
		[&](Connection* pcon) {
			std::unique_lock<std::mutex> lock(_queueMutex);
			pcon->refreshAliveTime();
			_connectionQue.push(pcon);
		});

	_connectionQue.pop();
	cv.notify_all();

	return sp;
}

void ConnectionPool::scanConnectionTask()
{
	for (;;)
	{
		std::this_thread::sleep_for(std::chrono::seconds(_maxIdleTime));

		std::unique_lock<std::mutex> lock(_queueMutex);
		while (_connectionCnt > _initSize)
		{
			Connection* p = _connectionQue.front();
			if (p->getAliveTime() <= _maxIdleTime * 1000)
				break;

			_connectionQue.pop();
			_connectionCnt--;
			delete p;
		}
	}
}

