#include <iostream>

#include "Connection.h"
#include "CommonConnectionPool.h"

int main()
{
	//ConnectionPool* cp = ConnectionPool::getConnectionPool();
	//cp->loadConfigFile();

	//std::clock_t begin = clock();
	//for (int i = 0; i < 1000; ++i)
	//{
	//	Connection conn;
	//	char sql[1024] = { 0 };
	//	sprintf(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//	conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
	//	conn.update(sql);
	//}
	//std::clock_t end = clock();

	//std::cout << (end - begin) << "ms" << std::endl;

	//std::clock_t begin2 = clock();
	//ConnectionPool* cp = ConnectionPool::getConnectionPool();
	//for (int i = 0; i < 1000; i++)
	//{
	//	std::shared_ptr<Connection> sp = cp->getConnection();
	//	char sql[1024] = { 0 };
	//	sprintf(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//	sp->update(sql);
	//}
	//std::clock_t end2 = clock();
	//std::cout << (end2 - begin2) << "ms" << std::endl;

	std::clock_t begin = clock();
	std::thread t1([] {
		for (int i = 0; i < 250; ++i)
		{
			//Connection conn;
			//char sql[1024] = { 0 };
			//sprintf(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
			//conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
			//conn.update(sql);

			ConnectionPool* cp = ConnectionPool::getConnectionPool();
			std::shared_ptr<Connection> sp = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
			sp->update(sql);
		}
		});

	std::thread t2([] {
		for (int i = 0; i < 250; ++i)
		{
			//Connection conn;
			//char sql[1024] = { 0 };
			//sprintf(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
			//conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
			//conn.update(sql);

			ConnectionPool* cp = ConnectionPool::getConnectionPool();
			std::shared_ptr<Connection> sp = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
			sp->update(sql);
		}
		});

	std::thread t3([] {
		for (int i = 0; i < 250; ++i)
		{
			//Connection conn;
			//char sql[1024] = { 0 };
			//sprintf(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
			//conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
			//conn.update(sql);

			ConnectionPool* cp = ConnectionPool::getConnectionPool();
			std::shared_ptr<Connection> sp = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
			sp->update(sql);
		}
		});

	std::thread t4([] {
		for (int i = 0; i < 250; ++i)
		{
			//Connection conn;
			//char sql[1024] = { 0 };
			//sprintf(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
			//conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
			//conn.update(sql);

			ConnectionPool* cp = ConnectionPool::getConnectionPool();
			std::shared_ptr<Connection> sp = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
			sp->update(sql);
		}
		});

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	std::clock_t end = clock();
	std::cout << (end - begin) << "ms" << std::endl;

	return 0;
}