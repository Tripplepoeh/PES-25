#ifndef GETFROMDATABASE_H
#define GETFROMDATABASE_H

#include "idatabase.h"

class getFromDatabase : public IDatabase{
	
public:
	getFromDatabase();
	~getFromDatabase();
	void dbInit(const std::string &host, const std::string& user, const std::string& password, const std::string& db);
	std::string krijgRecentsteWaardeUitDatabase(std::string sensorNaam);
	bool checkWaardeInDatabase(std::string sensorNaam, double  sensorWaarde);
	
private:
MYSQL* database;
bool verbonden;
	
};

#endif
