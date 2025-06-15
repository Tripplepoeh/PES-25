#ifndef IDATABASE_H
#define IDATABASE_H

#include <string>
#include <vector>
#include <mariadb/mysql.h>

class IDatabase{
	
public:
	virtual ~IDatabase(){};
	virtual void dbInit(const std::string &host, const std::string& user, const std::string& password, const std::string& db) = 0;
	
};

#endif
