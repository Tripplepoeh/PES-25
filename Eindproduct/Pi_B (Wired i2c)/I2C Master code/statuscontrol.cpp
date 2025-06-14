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

static int bewegingsTeller = 0;

statuscontrole::statuscontrole(IDatabase* log, IDatabase* get) 
    : logger(log), getDatabase(get), teller(0) 
{
    // Initialiseer sensors en actuatoren met waarde 0 en de juiste id volgens wat er staat in alleDefines.h
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
	 // Geef de response van de verwerkte binnenkomende data terug aan de client
    return responseBuffer;
}

void statuscontrole::procesData(const uint8_t* data, size_t length) {
    responseBuffer.clear();
    std::cout<<data << std::endl;
    size_t i = 0;
	// Eerst alle sensorupdates verwerken totdat een get/set commando wordt gevonden
    while (i < length && data[i] != GET_WAARDE && data[i] != SET_ACTUATOR)
        processSensorUpdates(data, i, length);
 // Verwerk de nieuwe statussen van actuators op basis van de nieuwe sensordata
    setActuators();
    // Verwerk eventuele GET/SET commando's die achter de sensordata binnenkomen
    while (i < length)
        processI2CCommands(data, i, length);
}

void statuscontrole::processSensorUpdates(const uint8_t* d, size_t& i, size_t n) {
    if (i >= n) return;
	//Krijg de id van de sensor die verwerkt wordt
    uint8_t id = d[i++];
	//Als id rfidsensor is moet je daarvoor 5 bytes vrijmaken anders 2 bytes aan waarde
    size_t len = (id == RFIDSENSOR ? 5 : 2);
    if (i + len > n) return;
	
    uint64_t val = 0;
    for (size_t b = 0; b < len; ++b) {
        val |= uint64_t(d[i + b]) << (8 * b);
    }
    i += len;
	//Krijg een pointer terug naar de sensor 
    sensor* it = returnSensor(id);
	//Ga verder als de pointer geldid is en geen null pointer is
    if (it) {
        if (id == RFIDSENSOR) {
            it->sensorWaarde = val;
	//Als de id van tempsensor of luchtvochtigheidsensor is zet dan de waarde terug naar een getal achter de komma
        }else if (id == TEMPSENSOR || id == LUCHTVSENSOR) {
            double fval = static_cast<double>(val) / 100.0;
            it->sensorWaarde = fval;
        } else {
            it->sensorWaarde = static_cast<double>(val);
        }
    }
}

statuscontrole::sensor* statuscontrole::returnSensor(const uint8_t& id){
	 // Zoek naar de sensor in de vector op basis van zijn id met find command en een lambda expressie
	auto it = std::find_if(sensorOpslag.begin(), sensorOpslag.end(),
								[&](const sensor& s){ return s.sensorId == id; });
	// Als de sensor gevonden is return dat anders stuur een null pointer terug
	if(it != sensorOpslag.end()) return &(*it);
	return nullptr;

} 



statuscontrole::actuator* statuscontrole::returnActuator(const  uint8_t& id){
	// Zoek naar de actuator in de vector op basis van zijn id met find command en een lambda expressie
	auto it = std::find_if(actuatoren.begin(), actuatoren.end(),
								[&](const actuator& s){ return s.actuatorId == id; });
	 // Als de actuator gevonden is return dat anders stuur een null pointer terug
	if(it != actuatoren.end()) return &(*it);
	return nullptr;

} 

void statuscontrole::processI2CCommands(const uint8_t* d, size_t& i, size_t n) {
    while (i < n) {
	    // Als de eerste token een get is en er is nog een id meegestuurd ga in deze if
        if (d[i] == GET_WAARDE && i + 1 < n) {
            ++i;
		//krijg de id van de get command
            uint8_t aid = d[i++];
		//als id speciaalbeheerdisplay is, moet de co2 waarde teruggegeven worden
            if (aid == SPECIALBEHEERDISPLAY) {
		    //Krjig pointer naar de co2sensor
                auto it = returnSensor(CO2SENSOR);
		    //Sla de co2waarde op 
                uint16_t v = it ? uint16_t(it->sensorWaarde) : 0;
		    //Plaats de speciaalbeheerdisplay respons in de buffer
                responseBuffer.insert(responseBuffer.end(), { aid, uint8_t(v), uint8_t(v >> 8) });
		    //Als de if lichtkrant is moet de menu teruggeven worden
            } else if (aid == LICHTKRANT) {
		    //Check of de getDatabase pointer geldig is
                if (getDatabase) {
                    std::string menu;
			//Check of de getDatabase pointer van de getFromDatabase child klasse is en sla de menu op
                    if (auto getCasted = dynamic_cast<getFromDatabase*>(getDatabase))
                        menu = getCasted->krijgRecentsteWaardeUitDatabase("lichtkrant");
                    static std::string lastMenu;
			//Stuur alleen de menu door als dit niet overeenkomt met de vorige menu
                    if (menu != lastMenu) {
                        lastMenu = menu;
			    //Plaats de lichtkrant respons in de buffer 
                        responseBuffer.push_back(aid);
                        for (char c : menu) {
                            responseBuffer.push_back(static_cast<uint8_t>(c));
                        }
			    //Deze 0 geeft aan de slave aan dat de menu voorbij is als een soort null terminator
                        responseBuffer.push_back(0);
                    }
                }
            } else {
		    //Voor overige actuators worden die statussen ook in de buffer gezet
                auto it = returnActuator(aid);
                uint8_t status = it ? it->actuatorStatus : 0;
                responseBuffer.insert(responseBuffer.end(), { aid, status });
            }
		//Mogelijkheid tot een actuator status te zetten maar dit wordt niet gebruikt
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
// Lambda expressie die de sensorwaarde teruggeeft als je get met de sensorId meegeeft anders wordt automatisch een 0 teruggegeven. De capture is een reference zodat hij bij de returnSensor functie kan komen
    auto get = [&](uint8_t id) -> double {
        auto it = returnSensor(id);
        if (it) return it->sensorWaarde;
        return 0.0;
    };
    // Lambda expressie die de actuatorwaarde aanpast van de actuator. De capture is een reference zodat hij bij de returnActuator functie kan komen
    auto setActuator = [&](uint8_t id, uint8_t val) {
        auto it = returnActuator(id);
        if (it) it->actuatorStatus = val;
    };

   // Zet roodLamp aan als co2sensor boven 500 is 
    setActuator(ROODLAMP, (get(CO2SENSOR) > 500));
    // Zet groenlamp aan als grondsensor boven 500 is 
    setActuator(GROENLAMP, (get(GRONDSENSOR) > 500));


    uint64_t rfidWaarde = get(RFIDSENSOR);
    std::string waardeInString =  std::to_string(rfidWaarde);
    //Check of de rfidwaarde die binnen is gekomen in de database voorkomt. Als dat zo zet de teller op 5
    double code =  get(RFIDSENSOR);
	if (auto getCasted = dynamic_cast<getFromDatabase*>(getDatabase))
    if(getCasted->checkWaardeInDatabase("rfid", code))
        teller = 5;

	//Houdt voor 5 iteraties lang dat de rfidnacheck op true 
	if (teller > 0) {
        returnSensor(RFIDNACHECK)->sensorWaarde = 1;
        teller--;
	} else {
		returnSensor(RFIDNACHECK)->sensorWaarde = 0;
	}
    
    std::cout<<waardeInString << " rfidwaarde " <<  teller<< std::endl; 
	//Open de deur als deurknop gedrukt is of een rfidwaarde in de database voorkwam
    setActuator(DEUR, get(DEURKNOP) || get(RFIDNACHECK));

  // Zet buzzer aan als noodknop gedrukt is
    setActuator(BUZZER, get(NOODKNOP));
    // Zet geellamp aan als noodknop gedrukt is
    setActuator(GEELLAMP, get(NOODKNOP));
        
    // Als bewegingsensor op 1 is onthoudt het minstens voor 5 iteraties zodat hij zeker weten het ledstripje aan zal zetten
    if (get(BEWEGINGSENSOR) == 1) {
        bewegingsTeller = 5;
    } else if (bewegingsTeller > 0) {
        bewegingsTeller--;
    }
    std::cout << " tbewgingeller " << bewegingsTeller << std::endl; 
    // Zet ledstrip uit als er geen druk, beweging of noodknop niet gedrukt is
    if (get(DRUKSENSOR) >= 100 && bewegingsTeller == 0 && get(NOODKNOP) == 0) {
        setActuator(LEDSTRIP, 0);
    // Zet ledstrip op dimmen als er wel druk, geen beweging of noodknop niet gedrukt is
    } else if (bewegingsTeller == 0 && get(NOODKNOP) == 0) {
        setActuator(LEDSTRIP, 2);
    // Zet ledstrip aan als er uiteindelijk wel beweging of noodknop gedrukt is
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
    // Snprintf voor string
    std::ostringstream fifoBericht;
    // Ga langs elke sensor in de vector
    for (auto& s : sensorOpslag) {
        // Als de sensorWaarde niet overeenkomt met de oude sensorwaarde stuur het door 
        if (s.sensorWaarde != s.vorigeSensorWaarde) {
            // Zet de double waarde om naar een string
            std::string waardeInString = std::to_string(s.sensorWaarde);
            // Zet het bericht in elkaar
            fifoBericht << "set " << s.sensorNaam << " " << waardeInString << "\n";
            // Log naar database indien logger beschikbaar
            if (auto logCasted = dynamic_cast<logToDatabase*>(logger))
                logCasted->logSensorWaarde(s.sensorNaam, s.sensorWaarde);
            // Verander de oude waarde naar de nieuwe waarde
            s.vorigeSensorWaarde = s.sensorWaarde;
        }
    }
    // Zet het in elkaar gezette bericht om naar een string
    std::string fifoBerichtString = fifoBericht.str();
    // Als het string bericht niet leeg is stuur het door naar de andere proces zodat hij het doorstuurt naar de andere pi
    if (!fifoBerichtString.empty())
        write(fifoWriteFd, fifoBerichtString.c_str(), fifoBerichtString.size());
}
