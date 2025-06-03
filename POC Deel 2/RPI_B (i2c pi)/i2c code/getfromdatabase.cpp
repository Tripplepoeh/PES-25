#include "getfromdatabase.h" 
#include <iostream>
#include <sstream>

getFromDatabase::getFromDatabase(const std::string &host, const std::string& user, const std::string& password, const std::string& db){
	database = mysql_init(nullptr);
	verbonden = mysql_real_connect(database, host.c_str(), user.c_str(), password.c_str(), db.c_str(), 0, nullptr, 0);
	if(!verbonden){
		std::cerr<<"mysql verbinding error"<<mysql_error(database) << std::endl;
		
	} 
}

getFromDatabase::~getFromDatabase(){
	if(database) mysql_close(database);
	}
	
std::string getFromDatabase::krijgMenu(){
    if(!verbonden) return "";

    std::ostringstream oss;
    oss << "SELECT menu FROM dagMenu ORDER BY timestamp DESC LIMIT 1";

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

