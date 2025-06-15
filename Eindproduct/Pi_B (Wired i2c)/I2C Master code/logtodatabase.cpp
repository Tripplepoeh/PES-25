#include "logtodatabase.h" 
#include <iostream>
#include <sstream>

logToDatabase::logToDatabase(){

}

logToDatabase::~logToDatabase(){
	if(database) mysql_close(database);
	}
	
void logToDatabase::dbInit(const std::string &host, const std::string& user, const std::string& password, const std::string& db){
	database = mysql_init(nullptr);
	//Maak een verbinding naar de database met de juiste waardes die in de database object wordt opgeslagen. Als het succesvol is zal verbonden true worden
	verbonden = mysql_real_connect(database, host.c_str(), user.c_str(), password.c_str(), db.c_str(), 0, nullptr, 0);
	if(!verbonden){
		std::cerr<<"mysql verbinding error"<<mysql_error(database) << std::endl;
		
	} 
}
	
void logToDatabase::logSensorWaarde(std::string naam, double waarde){
	//Als er geen verbinding is met de database ga terug
	if(!verbonden) return;
	//Maak een output string wat gebruikt om de query te schrijven
	std::ostringstream oss;
	//Zet de sensorwaarde om naar een string
	std::string waardeInString =  std::to_string(waarde);
	//Maak de query om de sensornaam met waarde op te slaan
	oss << "INSERT INTO sensorlog (sensornaam, waarde) VALUES ('" << naam << "', " << waardeInString << ")";
	//Insert de query in de database en check of het goed gaat
	if(mysql_query(database, oss.str().c_str())){
		std::cerr<<"mysql insert error" << std::endl;
	}
}

