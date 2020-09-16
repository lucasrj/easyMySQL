#pragma once
#include <mysql/mysql.h>
#include <string>
#include <vector>
#include <ostream>

typedef std::vector<std::vector<std::string>> table;

class easymySQL
{
public:
    easymySQL(const char* host, const char* user, const char* pass, const char* db);
    easymySQL(const char* host, const char* user, const char* pass, const char* db,int port);

    table getTable(std::string tablename);
	void makeTable(std::string tablename,std::string setup);
	void dropTable(std::string tablename);
	
	void addData(std::string tablename, std::string data);
    void addData(std::string tablename, table data);



	void printTable(std::string tablename, std::ostream& ost);

	



private:
	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;
	std::string database;

};

