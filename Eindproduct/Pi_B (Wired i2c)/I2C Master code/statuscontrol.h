
#ifndef STATUSCONTROL_H
#define STATUSCONTROL_H
#include "istatuscontrole.h"

// Paden van de fifo's voor communicatie met socket proces
#define FIFO_READ  "/tmp/pi_to_i2cmaster"
#define FIFO_WRITE "/tmp/i2cmaster_to_pi"

//Forward declaration voor de database implementatie klassen
class IDatabase;

class statuscontrole : public istatuscontrole {
public:
	//Krijg de database child klassen binnen
	statuscontrole(IDatabase* log, IDatabase* get);

	//Procesdata wordt aangeroepen door de wemossocket object om binnenkomende data te verwerken
   	void procesData(const uint8_t* data, size_t length);
	//Leesfifo leest berichten die binnenkomen van de andere proces van de sockets
	void leesFifo();
	//Stuurberichtfifo stuurt berichten over sensorupdates naar de andere proces van de sockets zodat hij dat door stuurt naar de andere pi
	void stuurBerichtFifo(); 
	//Wemossocket object roept deze functie aan om de respons terug te krijgen naar wat hij moet sturen naar de clients. De const is er zodat het bericht niet meer gewijzigd wordt
	const std::vector<uint8_t>& getResponse() const;

private:
//Struct van een sensor met de benodigde data over die sensor 
	struct sensor {
		uint8_t sensorId;
		std::string sensorNaam;
		double sensorWaarde;
		double vorigeSensorWaarde;
        uint64_t rawWaarde;
	};
//Struct van een actuator met de benodigde data over die sensor 
	struct actuator {
		uint8_t actuatorId;
		std::string actuatorNaam;
		uint8_t actuatorStatus;
	};

    //Opslag plaats van sensor zodat ze makkelijk te vinden zijn in een vector
    std::vector<sensor> sensorOpslag;  
    //Opslag plaats van actuator zodat ze makkelijk te vinden zijn in een vector
    std::vector<actuator> actuatoren;
    //De responsebuffer waarin de response staat van de verwerkte data dat terug gestuurd moet worden naar clients
    std::vector<uint8_t> responseBuffer;
    //Enkele objecten voor fifo file descriptors en een teller wat gebruikt wordt om verwerkte rfiddata langer opgeslagen te houden
    int fifoReadFd, fifoWriteFd, teller;
    //Functie om alle actuatoren statussen te updaten op basis van sensordata
    void setActuators();
//Functie die sensorwaardes update op bais van de binnenkomende data
	void processSensorUpdates(const uint8_t* data, size_t& i, size_t len);
//Functie die get commandos van de client verwerkt en in de responsebuffer plaatst  
	void processI2CCommands(const uint8_t* data, size_t& i, size_t len);
   //Functie waarmee er een actuator* wordt gerouteerneerd om makkelijk actuators te vinden op basis van de actuatornaam
    wemosStatuscontrole::actuator* returnActuator(const std::string& zoekNaam);
    //Functie waarmee er een sesnor* wordt gerouteerneerd om makkelijk sensors te vinden op basis van de sensornaam
    wemosStatuscontrole::sensor* returnSensor(const std::string& zoekNaam);

    //Database object om te kunnen loggen
	IDatabase* logger;
 //Database object om waardes op te halen of te checken
	IDatabase* getDatabase;
};

#endif // STATUSCONTROL_H
