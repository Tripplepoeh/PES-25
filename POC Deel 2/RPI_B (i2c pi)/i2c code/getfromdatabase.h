#ifndef GETFROMDATABASE_H
#define GETFROMDATABASE_H

#include <string>
#include <vector>
#include <mariadb/mysql.h>

class getFromDatabase{
	
public:
	getFromDatabase(const std::string &host, const std::string& user, const std::string& password, const std::string& db);
	~getFromDatabase();
	std::string krijgMenu();
	
private:
MYSQL* database;
bool verbonden;
	
};

#endif
