#ifndef GETFROMDATABASE_H
#define GETFROMDATABASE_H

#include "idatabase.h"

class getFromDatabase : public IDatabase{
	
public:
	getFromDatabase();
	~getFromDatabase();
//Stuur alle belangrijke informatie van de database door zodat deze klasse toegang heeft tot de database
	void dbInit(const std::string &host, const std::string& user, const std::string& password, const std::string& db);
//Functie die de recentste menu teruggeeft
	std::string krijgRecentsteWaardeUitDatabase(std::string sensorNaam);
//Functie die checkt of de rfidwaarde voorkomt in de database
	bool checkWaardeInDatabase(std::string sensorNaam, double  sensorWaarde);
	
private:
//MYSQL database object waarmee met de database gewerkt kan worden met code
MYSQL* database;
//Bool die aangeeft of de connectie met de database klopt 
bool verbonden;
};

#endif
