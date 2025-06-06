#ifndef GETFROMDATABASE_H
#define GETFROMDATABASE_H

#include <string>
#include <vector>
#include <mariadb/mysql.h>

class getFromDatabase{
	
public:
	getFromDatabase(const std::string &host, const std::string& user, const std::string& password, const std::string& db);
	~getFromDatabase();
	bool checkRFID(uint64_t  rfidwaarde);
	
private:
MYSQL* database;
bool verbonden;
	
};

#endif
