#include "wemosStatuscontrole.h"
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include "getfromdatabase.h"
#define GA_UIT 0
#define GA_AAN 1
static int bewegingsTeller = 0;


wemosStatuscontrole::wemosStatuscontrole() : fifoReadFd(0), fifoWriteFd(0), teller(0){
	    // Initialiseer sensors en actuatoren in volgorde van gebruik
    sensorOpslag = {
        {"deurknop",      0, 0},
        { "noodknop",      0, 0},
        {"buzzerknop",    0, 0},
        {"druksensor",    0, 0},
        {"grondsensor",   0, 0},
        {"tempsensor",    0, 0},
        {"rfidsensor",    0, 0},
        {"co2sensor",     0, 0},
        {"bewegingsensor",0, 0},
        { "luchtvsensor",  0, 0},
        { "rfidNaCheck",  0, 0}
    };

    actuatoren = {
        {"roodlamp",             GA_UIT},
        {"groenlamp",            GA_UIT},
        { "deurservo",            GA_UIT},
        {"lichtkrant",           GA_UIT},
        { "ledstrip",             GA_UIT},
        {"deur",                 GA_UIT},
        {"specialbeheerdisplay",  GA_UIT},
        {"buzzer",               GA_UIT},
        {"geellamp",             GA_UIT}
    };

    // Maak een fifo pipes aan voor zowel schrijven als lezen en wacht tot de andere kant ook open is
    mkfifo(FIFO_WRITE, 0666);
    mkfifo(FIFO_READ, 0666);
    while ((fifoWriteFd = open(FIFO_WRITE, O_WRONLY | O_NONBLOCK)) < 0) usleep(100000);
    while ((fifoReadFd = open(FIFO_READ, O_RDONLY | O_NONBLOCK)) < 0) usleep(100000);
}

wemosStatuscontrole::~wemosStatuscontrole(){
	if (fifoReadFd >= 0) close(fifoReadFd);
    if (fifoWriteFd >= 0) close(fifoWriteFd);
    unlink(FIFO_READ);
    unlink(FIFO_WRITE);
    
}

const std::vector<uint8_t>& wemosStatuscontrole::getResponse() const{
		return responseBuffer;
}

/*void wemosStatuscontrole::procesData(const uint8_t* input, size_t length){
    responseBuffer.clear();
    // Maak van de input een string en verwijder \r, maar laat \n staan zodat we per regel kunnen parsen
    std::string cleanedInput;
    for (size_t i = 0; i < length; ++i) {
        if (input[i] != '\r') cleanedInput += input[i];
    }

    std::istringstream iss(cleanedInput);
    std::string line;
    while (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        std::string cmd;
        lineStream >> cmd;
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

        if (cmd == "set") {
            std::string sensorNaam;
            double waarde;
            if (lineStream >> sensorNaam >> waarde) {
				std::string waardeInString =  std::to_string(waarde);
                std::cout << "Set commando ontvangen: sensor = " << sensorNaam << ", waarde = " << waardeInString << std::endl;
                processSensorUpdates(sensorNaam.c_str(), waarde);
                setActuators();
                stuurBerichtFifo();
            } else {
                std::cout << "Fout bij het lezen van set commando" << std::endl;
            }
        } else if (cmd == "get") {
            std::string actuatorNaam;
            if (lineStream >> actuatorNaam) {
                setActuators();
                processWemosGetCommand(actuatorNaam.c_str());
            }
        }
    }
    stuurBerichtFifo();
}*/

void wemosStatuscontrole::procesData(const uint8_t* input, size_t length){
    responseBuffer.clear();

    // Maak van de input een string en verwijder \r, maar laat \n staan
    std::string cleanedInput;
    for (size_t i = 0; i < length; ++i) {
        if (input[i] != '\r' && input[i] != '\n') cleanedInput += input[i];
    }

    // Splits op witruimte in losse woorden/tokens
    std::istringstream iss(cleanedInput);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        std::transform(token.begin(), token.end(), token.begin(), ::tolower);
        tokens.push_back(token);
    }

    // Loop door tokens en verwerk set/get commando's
    size_t i = 0;
    while (i < tokens.size()) {
        if (tokens[i] == "set" && i+2 < tokens.size()) {
            std::string sensorNaam = tokens[i+1];
            double waarde = 0;
            try {
                waarde = std::stod(tokens[i+2]);
            } catch (...) {
                // Ongeldige waarde, skip dit commando
                i += 3;
                continue;
            }
            std::cout << "Set commando ontvangen: sensor = " << sensorNaam << ", waarde = " << waarde << std::endl;
            processSensorUpdates(sensorNaam.c_str(), waarde);
            setActuators();
            stuurBerichtFifo();
            i += 3;
        } else if (tokens[i] == "get" && i+1 < tokens.size()) {
            std::string actuatorNaam = tokens[i+1];
            setActuators();
            processWemosGetCommand(actuatorNaam.c_str());
            i += 2;
        } else {
            // Onbekende token, ga door naar volgende
            ++i;
        }
    }
    stuurBerichtFifo();
}

void wemosStatuscontrole::processSensorUpdates(const char* inputSensor, double inputWaarde){
	   // char message[512];
	   
	   sensor* sensorNieuweWaarde = returnSensor(inputSensor);
	if(sensorNieuweWaarde){
		if(sensorNieuweWaarde->sensorWaarde != inputWaarde){
			//sensorNieuweWaarde->vorigeSensorWaarde = sensorNieuweWaarde->sensorWaarde;
			sensorNieuweWaarde->sensorWaarde = inputWaarde;
		}
	}
}
	   
void wemosStatuscontrole::processWemosGetCommand(const char* data){
	std::ostringstream respons;
	actuator* krijgActuator = returnActuator(data);
	if(krijgActuator){
		int waarde = krijgActuator->actuatorStatus;
		std::string outputWaarde = (waarde == 0) ? "uit" : (waarde == 1) ? "aan" : "speciaal";
		respons << krijgActuator->actuatorNaam << ": " << outputWaarde << "\n";
		std::string stringRespons = respons.str();
		for (char c : stringRespons) {
                        responseBuffer.push_back(static_cast<uint8_t>(c));
                    }
		
	}
}

wemosStatuscontrole::sensor* wemosStatuscontrole::returnSensor(const std::string& zoekNaam){
	auto it = std::find_if(sensorOpslag.begin(), sensorOpslag.end(),
								[&](const sensor& s){ return s.sensorNaam == zoekNaam; });
	if(it != sensorOpslag.end()) return &(*it);
	return nullptr;

} 



wemosStatuscontrole::actuator* wemosStatuscontrole::returnActuator(const std::string& zoekNaam){
	auto it = std::find_if(actuatoren.begin(), actuatoren.end(),
								[&](const actuator& s){ return s.actuatorNaam == zoekNaam; });
	if(it != actuatoren.end()) return &(*it);
	return nullptr;

} 


void wemosStatuscontrole::setActuators(){
    auto get = [&](const std::string& zoekNaam) -> double {
		auto it = returnSensor(zoekNaam);
		 if (it) return it->sensorWaarde;
         return 0.0;
	};

    auto setActuator = [&](const std::string& actuatorNaam, uint8_t val) {
        auto it = returnActuator(actuatorNaam);
        if (it)
            it->actuatorStatus = val;
    };

    setActuator("roodlamp", (get("co2sensor") > 500));
    setActuator("groenlamp", (get("grondsensor") > 500));
    setActuator("geellamp", get("noodknop"));
    


    
    setActuator("deur", get("deurknop") || get("rfidNaCheck")  );
    
    setActuator("buzzer", get("noodknop"));

    
       if (get("bewegingsensor") == 1) {
        bewegingsTeller = 5;
    } else if (bewegingsTeller > 0) {
        bewegingsTeller--;
    }

    if (get("druksensor") >= 100 && bewegingsTeller == 0 && get("noodknop") == 0) {
        setActuator("ledstrip", 0);
    } else if (bewegingsTeller == 0 && get("noodknop") == 0) {
        setActuator("ledstrip", 2);
    } else {
        setActuator("ledstrip", 1);
    }
}


void wemosStatuscontrole::leesFifo(){
	char buffer[512];
    ssize_t bytesRead = read(fifoReadFd, buffer, sizeof(buffer) - 1);

    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        procesData(reinterpret_cast<const uint8_t*>(buffer), bytesRead);
    }
}

void wemosStatuscontrole::stuurBerichtFifo(){ 
    std::ostringstream fifoBericht;
    for (auto& s : sensorOpslag) {
		if (s.sensorWaarde != s.vorigeSensorWaarde) {
			std::string waardeInString =  std::to_string(s.sensorWaarde);
            fifoBericht << "set " << s.sensorNaam << " " << waardeInString << "\n";
            s.vorigeSensorWaarde = s.sensorWaarde;
    }
}
    std::string fifoBerichtString = fifoBericht.str();
    if (!fifoBerichtString.empty()) write(fifoWriteFd, fifoBerichtString.c_str(), fifoBerichtString.size());
    std::cout<<fifoBerichtString<<std::endl;
}

