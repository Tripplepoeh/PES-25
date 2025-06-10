#include "statuscontrol.h"
#include <fcntl.h>
#include <unistd.h>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <cctype>
#include <sys/stat.h>
#include <iostream>
#include "logtodatabase.h"
#include "getfromdatabase.h"
#include "alleDefines.h"
#include <cmath>
#include <iomanip>


static int bewegingsTeller = 0;

statuscontrole::statuscontrole(IDatabase* log, IDatabase* get) 
    : logger(log), getDatabase(get), teller(0) 
{
    // Initialiseer sensors en actuatoren in volgorde van gebruik
    sensorOpslag = {
        {DEURKNOP,      "deurknop",      0, 0, 0},
        {NOODKNOP,      "noodknop",      0, 0, 0},
        {BUZZERKNOP,    "buzzerknop",    0, 0, 0},
        {DRUKSENSOR,    "druksensor",    0, 0, 0},
        {GRONDSENSOR,   "grondsensor",   0, 0, 0},
        {TEMPSENSOR,    "tempsensor",    0, 0, 0},
        {RFIDSENSOR,    "rfidsensor",    0, 0, 0},
        {CO2SENSOR,     "co2sensor",     0, 0, 0},
        {BEWEGINGSENSOR,"bewegingsensor",0, 0, 0},
        {LUCHTVSENSOR,  "luchtvsensor",  0, 0, 0},
        {RFIDNACHECK, "rfidNaCheck",  0, 0, 0}
    };

    actuatoren = {
        {ROODLAMP,              "roodlamp",             GA_UIT},
        {GROENLAMP,             "groenlamp",            GA_UIT},
        {DEURSERVO,             "deurservo",            GA_UIT},
        {LICHTKRANT,            "lichtkrant",           GA_UIT},
        {LEDSTRIP,              "ledstrip",             GA_UIT},
        {DEUR,                  "deur",                 GA_UIT},
        {SPECIALBEHEERDISPLAY,  "specialbeheerdisplay",  GA_UIT},
        {BUZZER,                "buzzer",               GA_UIT},
        {GEELLAMP,              "geellamp",             GA_UIT}
    };

    // Maak een fifo pipes aan voor zowel schrijven als lezen en wacht tot de andere kant ook open is
    mkfifo(FIFO_WRITE, 0666);
    mkfifo(FIFO_READ, 0666);
    while ((fifoWriteFd = open(FIFO_WRITE, O_WRONLY | O_NONBLOCK)) < 0) usleep(100000);
    while ((fifoReadFd = open(FIFO_READ, O_RDONLY | O_NONBLOCK)) < 0) usleep(100000);
}

const std::vector<uint8_t>& statuscontrole::getResponse() const {
    return responseBuffer;
}

void statuscontrole::procesData(const uint8_t* data, size_t length) {
    responseBuffer.clear();
    std::cout<<data << std::endl;
    size_t i = 0;
    while (i < length && data[i] != GET_WAARDE && data[i] != SET_ACTUATOR)
        processSensorUpdates(data, i, length);

    setActuators();

    while (i < length)
        processI2CCommands(data, i, length);
}

void statuscontrole::processSensorUpdates(const uint8_t* d, size_t& i, size_t n) {
    if (i >= n) return;
    uint8_t id = d[i++];
    size_t len = (id == RFIDSENSOR ? 5 : 2);
    if (i + len > n) return;

    uint64_t val = 0;
    for (size_t b = 0; b < len; ++b) {
        val |= uint64_t(d[i + b]) << (8 * b);
    }
    i += len;

    sensor* it = returnSensor(id);
    if (it) {
        if (id == RFIDSENSOR) {
            it->rawWaarde = val;  // Sla raw uint64_t op
            it->sensorWaarde = val;
            std::cout<<"rfid " << val << std::endl; 
          //  it->sensorWaarde = static_cast<double>(val);
        }else if (id == TEMPSENSOR || id == LUCHTVSENSOR) {
            double fval = static_cast<double>(val) / 100.0;
            it->sensorWaarde = fval;
        } else {
            it->sensorWaarde = static_cast<double>(val);
        }
    }
}

statuscontrole::sensor* statuscontrole::returnSensor(const uint8_t& id){
	auto it = std::find_if(sensorOpslag.begin(), sensorOpslag.end(),
								[&](const sensor& s){ return s.sensorId == id; });
	if(it != sensorOpslag.end()) return &(*it);
	return nullptr;

} 



statuscontrole::actuator* statuscontrole::returnActuator(const  uint8_t& id){
	auto it = std::find_if(actuatoren.begin(), actuatoren.end(),
								[&](const actuator& s){ return s.actuatorId == id; });
	if(it != actuatoren.end()) return &(*it);
	return nullptr;

} 

/*void statuscontrole::processI2CCommands(const uint8_t* d, size_t& i, size_t n) {
    if (d[i] == GET_WAARDE && ++i < n) {
        uint8_t aid = d[i++];
        if (aid == SPECIALBEHEERDISPLAY) {
            auto it = returnSensor(CO2SENSOR);
            uint16_t v = it ? uint16_t(it->sensorWaarde) : 0;
            responseBuffer.insert(responseBuffer.end(), { aid, uint8_t(v), uint8_t(v >> 8) });
        }
        else if (aid == LICHTKRANT) {
            if (getDatabase) {
                std::string menu;
                if (auto getCasted = dynamic_cast<getFromDatabase*>(getDatabase)) menu = getCasted->krijgRecentsteWaardeUitDatabase("lichtkrant");
                static std::string lastMenu;
                if (menu != lastMenu) {
                    lastMenu = menu;
                    responseBuffer.push_back(aid);
                    for (char c : menu) {
                        responseBuffer.push_back(static_cast<uint8_t>(c));
                    }
                    responseBuffer.push_back(0);
                }
            }
        }
        else {
            auto it = returnActuator(aid);
            uint8_t status = it ? it->actuatorStatus  : 0;
            responseBuffer.insert(responseBuffer.end(), { aid, status });
        }
    }
    ++i;
}*/

void statuscontrole::processI2CCommands(const uint8_t* d, size_t& i, size_t n) {
    while (i < n) {
        if (d[i] == GET_WAARDE && i + 1 < n) {
            ++i;
            uint8_t aid = d[i++];
            if (aid == SPECIALBEHEERDISPLAY) {
                auto it = returnSensor(CO2SENSOR);
                uint16_t v = it ? uint16_t(it->sensorWaarde) : 0;
                responseBuffer.insert(responseBuffer.end(), { aid, uint8_t(v), uint8_t(v >> 8) });
            } else if (aid == LICHTKRANT) {
                if (getDatabase) {
                    std::string menu;
                    if (auto getCasted = dynamic_cast<getFromDatabase*>(getDatabase))
                        menu = getCasted->krijgRecentsteWaardeUitDatabase("lichtkrant");
                    static std::string lastMenu;
                    if (menu != lastMenu) {
                        lastMenu = menu;
                        responseBuffer.push_back(aid);
                        for (char c : menu) {
                            responseBuffer.push_back(static_cast<uint8_t>(c));
                        }
                        responseBuffer.push_back(0);
                    }
                }
            } else {
                auto it = returnActuator(aid);
                uint8_t status = it ? it->actuatorStatus : 0;
                responseBuffer.insert(responseBuffer.end(), { aid, status });
            }
        } else if (d[i] == SET_ACTUATOR && i + 2 < n) {
            auto it = returnActuator(d[i + 1]);
            if (it) it->actuatorStatus = d[i + 2];
            i += 3;
        } else {
            ++i;
        }
    }
}


void statuscontrole::setActuators() {
    auto get = [&](uint8_t id) -> double {
        auto it = returnSensor(id);
        if (it) return it->sensorWaarde;
        return 0.0;
    };

    auto setActuator = [&](uint8_t id, uint8_t val) {
        auto it = returnActuator(id);
        if (it) it->actuatorStatus = val;
    };


    setActuator(ROODLAMP, (get(CO2SENSOR) > 450));
    setActuator(GROENLAMP, (get(GRONDSENSOR) > 500));
    setActuator(DEURSERVO, get(DEURKNOP));
    setActuator(LEDSTRIP, !get(BEWEGINGSENSOR));

    uint64_t rfidWaarde = get(RFIDSENSOR);
    std::string waardeInString =  std::to_string(rfidWaarde);
    
    double code =  get(RFIDSENSOR);
	if (auto getCasted = dynamic_cast<getFromDatabase*>(getDatabase))
    if(getCasted->checkWaardeInDatabase("rfid", code))
        teller = 5;

	if (teller > 0) {
        returnSensor(RFIDNACHECK)->sensorWaarde = 1;
        teller--;
	} else {
		returnSensor(RFIDNACHECK)->sensorWaarde = 0;
        //teller = 0;
	}
    
    std::cout<<waardeInString << " rfidwaarde " <<  teller<< std::endl; 
    setActuator(DEUR, get(DEURKNOP) || get(RFIDNACHECK));

    setActuator(BUZZER, get(NOODKNOP));
    setActuator(GEELLAMP, get(NOODKNOP));
        
       if (get(BEWEGINGSENSOR) == 1) {
        bewegingsTeller = 5;
    } else if (bewegingsTeller > 0) {
        bewegingsTeller--;
    }
    std::cout << " tbewgingeller " <<  bewegingsTeller<< std::endl; 
    if (get(DRUKSENSOR) >= 100 && bewegingsTeller == 0 && get(NOODKNOP) == 0) {
        setActuator(LEDSTRIP, 0);
    } else if (bewegingsTeller == 0 && get(NOODKNOP) == 0) {
        setActuator(LEDSTRIP, 2);
    } else {
        setActuator(LEDSTRIP, 1);
    }

}

void statuscontrole::leesFifo() {
    char buf[512];
    ssize_t len = read(fifoReadFd, buf, sizeof(buf) - 1); // lees uit de fifo pipe
    if (len <= 0) return; // als fifo leeg is, return

    buf[len] = '\0'; // null-terminator toevoegen
    std::cout << "FIFO Input: " << buf << std::endl;

    // Input opschonen van \r en \n
    std::string cleanedInput;
    for (int i = 0; i < len; ++i) {
        if (buf[i] != '\r' && buf[i] != '\n') {
            cleanedInput += buf[i];
        }
    }
    std::cout<<cleanedInput<<std::endl;

    // Stringstream gebruiken om door tokens te gaan
    std::istringstream iss(cleanedInput);
    std::string token;

    while (iss >> token) {
        std::transform(token.begin(), token.end(), token.begin(), ::tolower);
        if (token == "set") {
            std::string sensorNaam;
            double waarde;

            if (!(iss >> sensorNaam >> waarde)) {
                std::cout << "Ongeldig set commando, ontbrekende waarden\n";
                continue;
            }

            std::string name = sensorNaam;
            uint64_t value = static_cast<uint64_t>(waarde);

            // Zoek sensor in de lijst
            auto it = std::find_if(sensorOpslag.begin(), sensorOpslag.end(),
                                   [&](const sensor& s) { return s.sensorNaam == name; });

            if (it == sensorOpslag.end()) {
                std::cout << "Sensor '" << name << "' niet gevonden\n";
                continue;
            }

            uint8_t id = it->sensorId;
            bool isRfid = (id == RFIDSENSOR);
            size_t payloadLen = isRfid ? 5 : 2;

            std::vector<uint8_t> data(1 + payloadLen);
            data[0] = id;
            for (size_t b = 0; b < payloadLen; ++b) {
                data[1 + b] = (value >> (8 * b)) & 0xFF;
            }

            size_t idx = 0;
            processSensorUpdates(data.data(), idx, data.size());
            setActuators();
             std::string waardeInString =  std::to_string(waarde);
            std::cout << "Set commando ontvangen: sensor = " << name << ", waarde = " << waardeInString << std::endl;
        }
    }
}

void statuscontrole::stuurBerichtFifo() {
    std::ostringstream fifoBericht;
    for (auto& s : sensorOpslag) {
        if (s.sensorWaarde != s.vorigeSensorWaarde) {
            std::string waardeInString =  std::to_string(s.sensorWaarde);
            fifoBericht << "set " << s.sensorNaam << " " << waardeInString << "\n";
            
            if(auto logCasted = dynamic_cast<logToDatabase*>(logger)) logCasted->logSensorWaarde(s.sensorNaam, s.sensorWaarde);
            s.vorigeSensorWaarde = s.sensorWaarde;
            
    }
}
    std::string fifoBerichtString = fifoBericht.str();
    if (!fifoBerichtString.empty()) write(fifoWriteFd, fifoBerichtString.c_str(), fifoBerichtString.size());
}
