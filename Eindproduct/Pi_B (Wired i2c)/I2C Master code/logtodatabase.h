#ifndef LOGTODATABASE_H
#define LOGTODATABASE_H

#include "idatabase.h"

class logToDatabase : public IDatabase{
	
public:
	logToDatabase();
	~logToDatabase();
	void dbInit(const std::string &host, const std::string& user, const std::string& password, const std::string& db);
	void logSensorWaarde(std::string naam, double waarde);
	
private:
MYSQL* database;
bool verbonden;
	
};

#endif
