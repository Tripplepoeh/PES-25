#include "getfromdatabase.h" 
#include <iostream>
#include <sstream>

getFromDatabase::getFromDatabase(){

}
void getFromDatabase::dbInit(const std::string &host, const std::string& user, const std::string& password, const std::string& db){
	database = mysql_init(nullptr);
	verbonden = mysql_real_connect(database, host.c_str(), user.c_str(), password.c_str(), db.c_str(), 0, nullptr, 0);
	if(!verbonden){
		std::cerr<<"mysql verbinding error"<<mysql_error(database) << std::endl;
		
	} 
}

getFromDatabase::~getFromDatabase(){
	if(database) mysql_close(database);
	}
	
std::string getFromDatabase::krijgRecentsteWaardeUitDatabase(std::string sensorNaam){
    if(!verbonden) return "";

    std::ostringstream oss;
    if(sensorNaam == "lichtkrant"){
		 oss << "SELECT menu FROM dagMenu ORDER BY timestamp DESC LIMIT 1";
	 }else{return "";}

    if(mysql_query(database, oss.str().c_str())){
        std::cerr << "MySQL query error: " << mysql_error(database) << std::endl;
        return "";
    }

    MYSQL_RES* result = mysql_store_result(database);
    if(!result){
        std::cerr << "MySQL fetch error: " << mysql_error(database) << std::endl;
        return "";
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    std::string menu;

    if(row && row[0]) {
        menu = row[0];  // menu is de eerste kolom
    }

    mysql_free_result(result);
    return menu;
}

bool getFromDatabase::checkWaardeInDatabase(std::string sensorNaam, double  sensorWaarde) {
    if (!verbonden) return false;

    std::ostringstream oss;
    std::string waardeInString =  std::to_string(sensorWaarde);
    oss << "SELECT * FROM " << sensorNaam << " WHERE rfidcode = " << waardeInString << " LIMIT 1";

    if (mysql_query(database, oss.str().c_str())) {
        std::cerr << "MySQL query error: " << mysql_error(database) << std::endl;
        return false;
    }

    MYSQL_RES* result = mysql_store_result(database);
    if (!result) {
        std::cerr << "MySQL fetch error: " << mysql_error(database) << std::endl;
        return false;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    bool found = false;

    if (row && row[0]) {
        int count = std::atoi(row[0]);
        found = (count > 0);
    }

    mysql_free_result(result);
    return found;
}

