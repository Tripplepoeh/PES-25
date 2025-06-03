#ifndef LOGTODATABASE_H
#define LOGTODATABASE_H

#include <string>
#include <vector>
#include <mariadb/mysql.h>

class logToDatabase{
	
public:
	logToDatabase(const std::string &host, const std::string& user, const std::string& password, const std::string& db);
	~logToDatabase();
	void logSensorWaarde(std::string naam, double waarde);
	
private:
MYSQL* database;
bool verbonden;
	
};

#endif
