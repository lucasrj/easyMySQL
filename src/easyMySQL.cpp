#include "easymySQL.h"
#include <mysql/mysql.h>
#include <string>
#include <iostream>
#include <vector>

easymySQL::easymySQL(const char *host, const char *user, const char *pass, const char *db)
{

	database = db;
	conn = mysql_init(0);
	conn = mysql_real_connect(conn, host, user, pass, db, 3306, NULL, 0);
	if (conn)
	{
		std::cout << "Succesful connection to database!" << std::endl;
	}
	else
		std::cout << "Connection to database has failed" << std::endl;
}

easymySQL::easymySQL(const char *host, const char *user, const char *pass, const char *db, int port)
{

	conn = mysql_init(0);
	conn = mysql_real_connect(conn, host, user, pass, db, port, NULL, 0);
	if (conn)
	{
		std::cout << "Succesful connection to database!" << std::endl;
	}
	else
		std::cout << "Connection to database has failed" << std::endl;
}

std::vector<std::vector<std::string>> easymySQL::getTable(std::string tablename)
{
	std::vector<std::vector<std::string>> table;
	std::string query = "SELECT COUNT(*) FROM INFORMATION_SCHEMA.COLUMNS WHERE table_schema = '" + database + "'AND table_name = '" + tablename + "';";
	const char *q = query.c_str();
	int qstate = mysql_query(conn, q);
	if (!qstate)
	{
		res = mysql_store_result(conn);
		row = mysql_fetch_row(res);
		std::string sTemp = row[0];
		int rows = std::stoi(sTemp);
		std::string query = "SELECT * FROM " + tablename;
		const char *q2 = query.c_str();
		qstate = mysql_query(conn, q2);
		if (!qstate)
		{
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res))
			{
				std::vector<std::string> temp;
				for (int i = 0; i < rows; ++i)
				{
					std::string str = row[i];
					for (int j = 0; j < str.length(); ++j)
					{
						switch ((int)str.at(j))
						{
						case -27:
							str.replace(str.begin()+j, str.begin()+j+1, "å");
							break;
						case -8:
							str.replace(str.begin()+j, str.begin()+j+1, "ø");
							break;
						case -26:
							str.replace(str.begin()+j, str.begin()+j+1, "æ");
							break;
						case -59:
							str.replace(str.begin()+j, str.begin()+j+1, "Å");
							break;
						case -40:
							str.replace(str.begin()+j, str.begin()+j+1, "Ø");
							break;
						case -58:
							str.replace(str.begin()+j, str.begin()+j+1, "Æ");
							break;
						}
					}
					
					temp.push_back(str);
				}
				table.push_back(temp);
			}
		}
		else
			std::cout << "Query failed: " << mysql_error(conn) << std::endl;
	}
	else
		std::cout << "Query failed: " << mysql_error(conn) << std::endl;

	return table;
}

void easymySQL::makeTable(std::string tablename, std::string setup)
{
	std::string query = "create table " + tablename + " (" + setup + ");";
	const char *q = query.c_str();
	int qstate = mysql_query(conn, q);
}

void easymySQL::dropTable(std::string tablename)
{

	std::string query = "drop table if exists " + tablename + ";";
	const char *q = query.c_str();
	int qstate = mysql_query(conn, q);
}

void easymySQL::addData(std::string tablename, std::string data)
{
	std::string query = "insert into " + tablename + " values (" + data + ");";
	const char *q = query.c_str();
	int qstate = mysql_query(conn, q);
}

void easymySQL::addData(std::string tablename, std::vector<std::vector<std::string>> data)
{
	std::string query = "insert into " + tablename + " ½values (";
	for (int i = 0; i < data.size(); ++i)
	{
		for (int j = 0; j < data[i].size(); ++j)
		{
			if (j == data[i].size() - 1)
				query = query + "\"" + data[i][j] + "\"";
			else
				query = query + "\"" + data[i][j] + "\",";
		}
		if (i == data.size() - 1)
			query = query + ");";
		else
			query = query + "),(";
	}
	std::cout << query;
	const char *q = query.c_str();
	int qstate = mysql_query(conn, q);
}

void easymySQL::printTable(std::string tablename, std::ostream &ost)
{
	std::vector<std::vector<std::string>> temp = this->getTable(tablename);

	ost << "table :" << tablename << std::endl;
	for (int i = 0; i < temp.size(); ++i)
	{
		for (int j = 0; j < temp[0].size(); j++)
		{
			ost << temp[i][j] << " ";
		}
		ost << std::endl;
	}
}
