#include "easymySQL.h"
#include <mysql/mysql.h>
#include <string>
#include <iostream>
#include <vector>

easymySQL::easymySQL(const char *host, const char *user, const char *pass, const char *db)
{

	database = db;
	conn = mysql_init(0);
	conn = mysql_real_connect(conn, host, user, pass, db, 3306, NULL, 0); //connects to database
}

easymySQL::easymySQL(const char *host, const char *user, const char *pass, const char *db, int port)
{

	conn = mysql_init(0);
	conn = mysql_real_connect(conn, host, user, pass, db, port, NULL, 0); //connects to database
}

bool easymySQL::isConnected()
{
	return conn; //returns if the database is connected
}

std::vector<std::vector<std::string>> easymySQL::getTable(std::string tablename)
{
	table dbtable;
	std::string query = "SELECT COUNT(*) FROM INFORMATION_SCHEMA.COLUMNS WHERE table_schema = '" + database + "'AND table_name = '" + tablename + "';";//asks for number of cols in table
	const char *q = query.c_str(); //convers query to c string
	int qstate = mysql_query(conn, q); // sends query and writes return code to qstarte
	if (!qstate)// ses if return code is good
	{
		res = mysql_store_result(conn);//writes the result of the query to res
		row = mysql_fetch_row(res);//writes the next line in res and writes it to row
		std::string sTemp = row[0];//converts first item in row to string
		int rows = std::stoi(sTemp);//converts to a string
		std::string query = "SELECT * FROM " + tablename; //asks for for the table
		const char *q2 = query.c_str();
		qstate = mysql_query(conn, q2);//sends query
		if (!qstate)
		{
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res))//takes the next row in res until it is emti
			{
				std::vector<std::string> temp;//makes temp
				for (int i = 0; i < rows; ++i)//loops thou all values in row
				{
					temp.push_back(row[i]);//adds value to temp
				}
				dbtable.push_back(temp);//adds temps values to dbtable
			}
		}
		else
			std::cout << "Query failed: " << mysql_error(conn) << std::endl;//prints to console if somthing vent wrong
	}
	else
		std::cout << "Query failed: " << mysql_error(conn) << std::endl;

	return dbtable;
}

void easymySQL::makeTable(std::string tablename, std::string setup)
{
	std::string query = "create table " + tablename + " (" + setup + ");";//makes a table based on the string you give
	const char *q = query.c_str();//makes it to c string
	int qstate = mysql_query(conn, q);//sends query to database
	if(qstate)std::cout << "Query failed: " << mysql_error(conn) << std::endl; //prins to user if error given
}

void easymySQL::dropTable(std::string tablename)
{

	std::string query = "drop table if exists " + tablename + ";";//makes drop table query
	const char *q = query.c_str();//makes it to c string
	int qstate = mysql_query(conn, q);//sends query to database
	if(qstate)std::cout << "Query failed: " << mysql_error(conn) << std::endl;//prins to user if error given
}

void easymySQL::addData(std::string tablename, std::string data)
{
	std::string query = "insert into " + tablename + " values (" + data + ");";//makes query based on sting given
	const char *q = query.c_str();//makes it to c string
	int qstate = mysql_query(conn, q);//sends query to database
	if(qstate)std::cout << "Query failed: " << mysql_error(conn) << std::endl;//prins to user if error given
}

void easymySQL::addData(std::string tablename, std::vector<std::vector<std::string>> data)
{
	std::string query = "insert into " + tablename + " values (";//makes start of query
	for (int i = 0; i < data.size(); ++i)//loops thou all vectors in table
	{
		for (int j = 0; j < data[i].size(); ++j)//loops thou all items in vector
		{
			if (j == data[i].size() - 1)//looks if it is the last vector in table
				query = query + "\"" + data[i][j] + "\"";
			else
				query = query + "\"" + data[i][j] + "\",";
		}
		if (i == data.size() - 1)//looks if it is the last vector in table
			query = query + ");";
		else
			query = query + "),(";
	}
	const char *q = query.c_str();//makes it to c string
	int qstate = mysql_query(conn, q);// send to database
	if(qstate)std::cout << "Query failed: " << mysql_error(conn) << std::endl;//prins to user if error given
}

void easymySQL::addData(std::string tablename, std::vector<std::string> data)
{
	std::string query = "insert into " + tablename + " values (";//makes start of query
	for (int i = 0; i < data.size(); ++i)//loops thou all items in vector
	{
		query = query + "\"" + data[i] + "\",";
	}
	query = query + ");";//adds end to query
	const char *q = query.c_str();//makes it to c string
	int qstate = mysql_query(conn, q);//send to database
	if(qstate)std::cout << "Query failed: " << mysql_error(conn) << std::endl;//prins to user if error given
}

void easymySQL::editindex(std::string tablename, std::string coll, std::string newdata, int index, std::string id)
{
	std::string query = "UPDATE " + tablename + " set " + coll + " = \"" + newdata + "\" where " + id + " = \"" + std::to_string(index) + "\";";//makes query based on data given
	const char *q = query.c_str();//make it to c string
	int qstate = mysql_query(conn, q);//send to database
	if(qstate)std::cout << "Query failed: " << mysql_error(conn) << std::endl;//prins to user if error given
}

void easymySQL::dropindex(std::string tablename, int index, std::string id)
{
	std::string query = "DELETE FROM " + tablename + " where " + id + " = \"" + std::to_string(index) + "\";";//makes query based on data given
	const char *q = query.c_str();//make it to c string
	int qstate = mysql_query(conn, q);//send to database
	if(qstate)std::cout << "Query failed: " << mysql_error(conn) << std::endl;//prins to user if error given
}

void easymySQL::printTable(std::string tablename, std::ostream &ost)
{
	std::vector<std::vector<std::string>> temp = this->getTable(tablename);//gets table from database

	ost << "table :" << tablename << std::endl;//prints table name to stream
	for (int i = 0; i < temp.size(); ++i)// loops thou all rows ind table
	{
		for (int j = 0; j < temp[0].size(); j++)//loops thou all items in row
		{
			ost << temp[i][j] << " ";//prints item
		}
		ost << std::endl;
	}
	ost.flush();//flushes stream
}

void easymySQL::TableTocsv(std::string tablename, std::ostream &ost)
{
	std::vector<std::vector<std::string>> temp = this->getTable(tablename);//gets table from database

	ost << "table ," << tablename << std::endl;//writes table name to stream
	for (int i = 0; i < temp.size(); ++i)//loop thou all rows in table
	{
		for (int j = 0; j < temp[0].size(); j++)//loops thou all items in row
		{
			ost << temp[i][j] << ", ";//writes item
		}
		ost << std::endl;
	}
	ost.flush();//flushes stream
}
