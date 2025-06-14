#ifndef LOGTODATABASE_H
#define LOGTODATABASE_H

#include "idatabase.h"

class logToDatabase : public IDatabase{
	
public:
	logToDatabase();
	~logToDatabase();
//Stuur alle belangrijke informatie van de database door zodat deze klasse toegang heeft 
	void dbInit(const std::string &host, const std::string& user, const std::string& password, const std::string& db);
//Functie waarmee sensorwaardes gelogd worden
	void logSensorWaarde(std::string naam, double waarde);
	
private:
//MYSQL database object waarmee met de database gewerkt kan worden met code
MYSQL* database;
//Bool die aangeeft of de connectie met de database klopt 
bool verbonden;
	
};

#endif
