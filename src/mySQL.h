#pragma once
#include <mysql.h>
#include <string>
#include <vector>
#include <ostream>
class mySQL
{
public:
	mySQL(const char* host, const char* user, const char* pass, const char* db);
	mySQL(const char* host, const char* user, const char* pass, const char* db,int port);

	std::vector<std::vector<std::string>> getTable(std::string tablename);
	void makeTable(std::string tablename,std::string setup);
	void dropTable(std::string tablename);
	
	void addData(std::string tablename, std::string data);



	void printTable(std::string tablename, std::ostream& ost);

	



private:
	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;
	std::string database;

};

