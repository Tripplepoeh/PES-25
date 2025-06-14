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
    // Initialiseer sensors en actuatoren met alle waardes 0
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
	//sluit alle fifo pipes als ze open waren
	if (fifoReadFd >= 0) close(fifoReadFd);
    if (fifoWriteFd >= 0) close(fifoWriteFd);
    unlink(FIFO_READ);
    unlink(FIFO_WRITE);
    
}

const std::vector<uint8_t>& wemosStatuscontrole::getResponse() const{
	//Geef de response van de verwerkte binnenkomende data terug aan de wemossocket object	
	return responseBuffer;
}

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
    //Plaats een voor een token die naar kleine letters wordt gezet in de tokens vector 
    while (iss >> token) {
        std::transform(token.begin(), token.end(), token.begin(), ::tolower);
        tokens.push_back(token);
    }

    // Loop door tokens en verwerk set/get commando's
    size_t i = 0;
    while (i < tokens.size()) {
	//Als token een set commandos is met nog een string als waarde ga in deze if statement
        if (tokens[i] == "set" && i+2 < tokens.size()) { 
		//Krijg de sensornaam van de binnenkomende set commando
            std::string sensorNaam = tokens[i+1];
            double waarde = 0;
		//Probeer de sensorwaarde naar een double om te zetten 
            try {
                waarde = std::stod(tokens[i+2]);
            } catch (...) { 
                // Ongeldig input, binnenkomende sensordata kan niet naar double worden omgezet
                i += 3;
                continue;
            }
            std::cout << "Set commando ontvangen: sensor = " << sensorNaam << ", waarde = " << waarde << std::endl;
	    //Stuur sensornaam met waarde door naar processSensorUpdates zodat deze goed wordt verwerkt. De functie verwacht een char* dus stuur het door als een c_str
            processSensorUpdates(sensorNaam.c_str(), waarde); 
		//Verwerk de nieuwe statussen van actuators op basis van de nieuwe sensordata
            setActuators();
		//Stuur de nieuwe sensordata door naar de andere pi 
            stuurBerichtFifo();
		//i gaat +3 zodat hij de volgende commando kan parsen
            i += 3;
		//Als de eerste token een get is en er is nog een string meegestuurd ga in deze if
        } else if (tokens[i] == "get" && i+1 < tokens.size()) {
		//Pak de actuatornaam 
            std::string actuatorNaam = tokens[i+1];
		//Verwerk eerst alle actuators voordat de status wordt gestuurd naar een client
            setActuators();
		//Laat deze functie de responsebuffer vullen op basis van de actuatornaam. De functie verwacht een char* dus stuur het door als een c_str
            processWemosGetCommand(actuatorNaam.c_str());
		//Ga door naar de volgende commando 
            i += 2;
        } else {
            // Onbekende token, ga door naar volgende
            ++i;
        }
    }
	//Stuur nog een keer alle berichten naar de andere pi
    stuurBerichtFifo();
}

void wemosStatuscontrole::processSensorUpdates(const char* inputSensor, double inputWaarde){
	 //Krijg een pointer terug naar de sensor in de vector via de returnSensor funcite met de sensornaam als parameter
	   sensor* sensorNieuweWaarde = returnSensor(inputSensor);
	//Als sensorNieuweWaarde geen null pointer is, bestaat de sensor en is het mogelijk om zjin waarde aan te passen
	if(sensorNieuweWaarde){
		//Pas alleen de sensorwaarde aan als het niet hetzelfde is als de eerder opgeslagen sensorwaarde
		if(sensorNieuweWaarde->sensorWaarde != inputWaarde){
			//Verander de sensor zijn waarde naar de binnenkomende data
			sensorNieuweWaarde->sensorWaarde = inputWaarde;
		}
	}
}
	   
void wemosStatuscontrole::processWemosGetCommand(const char* data){
	//ostringstream is vergelijkend met snprintf maar voor strings
	std::ostringstream respons;
	//Krijg de actuator pointer van de binnenkomende actuatornaam via returnActuator functie met de naam als parameter
	actuator* krijgActuator = returnActuator(data);
	//Als de actuator pointer geen null pointer is ga verder
	if(krijgActuator){
		//Krijg de waarde van de actuator
		int waarde = krijgActuator->actuatorStatus;
		//Op basis van de actuatorwaarde pas de outputwaarde aan 
		std::string outputWaarde = (waarde == 0) ? "uit" : (waarde == 1) ? "aan" : "speciaal";
		//Zet de responsstring in elkaar 
		respons << krijgActuator->actuatorNaam << ": " << outputWaarde << "\n";
		//Zet de respons om naar een string zodat deze in de buffer geplaatst kan worden
		std::string stringRespons = respons.str();
		//Zet karakter voor karakter de stringRespons in de responseBuffer vector
		for (char c : stringRespons) {
                        responseBuffer.push_back(static_cast<uint8_t>(c));
                    }
		
	}
}

wemosStatuscontrole::sensor* wemosStatuscontrole::returnSensor(const std::string& zoekNaam){
	//Zoek naar de sensor  in de vector op basis van zijn naam met find command en een lambda expressie
	auto it = std::find_if(sensorOpslag.begin(), sensorOpslag.end(),
								[&](const sensor& s){ return s.sensorNaam == zoekNaam; });
	//Als de sensor gevonden is return dat anders stuur een null pointer terug
	if(it != sensorOpslag.end()) return &(*it);
	return nullptr;

} 



wemosStatuscontrole::actuator* wemosStatuscontrole::returnActuator(const std::string& zoekNaam){
	//Zoek naar de actuator in de vector op basis van zijn naam met find command en een lambda expressie
	auto it = std::find_if(actuatoren.begin(), actuatoren.end(),
								[&](const actuator& s){ return s.actuatorNaam == zoekNaam; });
	//Als de actuator gevonden is return dat anders stuur een null pointer terug
	if(it != actuatoren.end()) return &(*it);
	return nullptr;

} 


void wemosStatuscontrole::setActuators(){
	//Lambda expressie die de sensorwaarde teruggeeft als je get met de sensornaam meegeeft anders wordt automatisch een 0 teruggegeven. De capture is een reference zodat hij bij de returnActuator functie kan komen
    auto get = [&](const std::string& zoekNaam) -> double {
		auto it = returnSensor(zoekNaam);
		 if (it) return it->sensorWaarde;
         return 0.0;
	};
	//Lambda expressie die de actuatorwaarde aan past van de actuator. De capture is een reference zodat hij bij de returnActuator functie kan komen
    auto setActuator = [&](const std::string& actuatorNaam, uint8_t val) {
        auto it = returnActuator(actuatorNaam);
        if (it)
            it->actuatorStatus = val;
    };
	//Zet roodLamp aan als co2sensor boven 500 is 
    setActuator("roodlamp", (get("co2sensor") > 500));
	//Zet groenlamp aan als grondsensor boven 500 is 
    setActuator("groenlamp", (get("grondsensor") > 500));
	//Zet geellamp aan als noodknop gedrukt is
    setActuator("geellamp", get("noodknop"));
    


    //Zet de deur aan als deurknop gedrukt is of als rfidsensor na een database check op de andere pi goedgekeurd is
    setActuator("deur", get("deurknop") || get("rfidNaCheck")  );
    //Zet buzzer aan als noodknop gedrukt is
    setActuator("buzzer", get("noodknop"));

    //Als bewegingsensor op 1 is onthoudt het minstens voor 5 iteraties zodat hij zeker weten het ledstripje aan zal zetten
       if (get("bewegingsensor") == 1) {
        bewegingsTeller = 5;
    } else if (bewegingsTeller > 0) {
        bewegingsTeller--;
    }
	//Zet ledstrip uit als er geen druk, beweging of noodknop niet gedrukt is
    if (get("druksensor") >= 100 && bewegingsTeller == 0 && get("noodknop") == 0) {
        setActuator("ledstrip", 0);
	    //Zet ledstrip op dimmen als er wel druk, geen beweging of noodknop niet gedrukt is
    } else if (bewegingsTeller == 0 && get("noodknop") == 0) {
        setActuator("ledstrip", 2);
	    //Zet ledstrip aan als er uiteindelijk wel beweging of noodknop gedrukt is
    } else {
        setActuator("ledstrip", 1);
    }
}


void wemosStatuscontrole::leesFifo(){
	char buffer[512];
	//Lees het bericht van de andere proces van de socket tussen pi's
    ssize_t bytesRead = read(fifoReadFd, buffer, sizeof(buffer) - 1);
	//Als er wel een bericht gelezen is null terminate het en een stuur het door naar procesdata
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        procesData(reinterpret_cast<const uint8_t*>(buffer), bytesRead);
    }
}

void wemosStatuscontrole::stuurBerichtFifo(){ 
	//Snprintf voor string
    std::ostringstream fifoBericht;
	//Ga langs elke sensor in de vector
    for (auto& s : sensorOpslag) {
	    //Als de sensorWaarde niet overeenkomt met de oude sensorwaarde stuur het door 
		if (s.sensorWaarde != s.vorigeSensorWaarde) {
			//Zet de double waarde om naar een string
			std::string waardeInString =  std::to_string(s.sensorWaarde);
			//Zet het bericht in elkaar
            fifoBericht << "set " << s.sensorNaam << " " << waardeInString << "\n";
			//Verander de oude waarde naar de nieuwe waarde
            s.vorigeSensorWaarde = s.sensorWaarde;
    }
}
	//Zet het in elkaar gezette bericht om naar een strint
    std::string fifoBerichtString = fifoBericht.str();
	//Als het string bericht niet leeg is stuur het door naar de andere proces zodat hij het doorstuurt naar de andere pi
    if (!fifoBerichtString.empty()) write(fifoWriteFd, fifoBerichtString.c_str(), fifoBerichtString.size());
    std::cout<<fifoBerichtString<<std::endl;
}

