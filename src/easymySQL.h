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

	bool isConnected();

    table getTable(std::string tablename);
	void makeTable(std::string tablename,std::string setup);
	void dropTable(std::string tablename);
	
	void addData(std::string tablename, std::string data);
    void addData(std::string tablename, table data);
	void addData(std::string tablename, std::vector<std::string> data);

    void editindex(std::string tablename,std::string coll, std::string newdata ,int index, std::string id = "id");
    void dropindex(std::string tablename, int index, std::string id = "id");

	table getTableNames();

	void printTable(std::string tablename, std::ostream& ost);
	void TableTocsv(std::string tablename, std::ostream& ost);

	



private:
	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;
	std::string database;

};

