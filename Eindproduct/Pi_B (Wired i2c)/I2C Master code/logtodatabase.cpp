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
	verbonden = mysql_real_connect(database, host.c_str(), user.c_str(), password.c_str(), db.c_str(), 0, nullptr, 0);
	if(!verbonden){
		std::cerr<<"mysql verbinding error"<<mysql_error(database) << std::endl;
		
	} 
}
	
void logToDatabase::logSensorWaarde(std::string naam, double waarde){
	if(!verbonden) return;
	std::ostringstream oss;
	std::string waardeInString =  std::to_string(waarde);
	oss << "INSERT INTO sensorlog (sensornaam, waarde) VALUES ('" << naam << "', " << waardeInString << ")";
	if(mysql_query(database, oss.str().c_str())){
		std::cerr<<"mysql insert error" << std::endl;
	}
}

