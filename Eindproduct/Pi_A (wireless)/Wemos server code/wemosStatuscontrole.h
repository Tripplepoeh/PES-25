
#ifndef STATUSCONTROL_H
#define STATUSCONTROL_H
#include "istatuscontrole.h"



#define FIFO_READ "/tmp/pi_to_wemosserver"
#define FIFO_WRITE "/tmp/wemosserver_to_pi"



class wemosStatuscontrole : public istatuscontrole {
public:
    wemosStatuscontrole();
    ~wemosStatuscontrole();
	
   	void procesData(const uint8_t* data, size_t length);
	void leesFifo();
	void stuurBerichtFifo(); 
	const std::vector<uint8_t>& getResponse() const;

private:	
struct sensor{
	std::string sensorNaam;
	double sensorWaarde;
	double vorigeSensorWaarde;
};

struct actuator{
	std::string actuatorNaam;
	uint8_t actuatorStatus;
};

    std::vector<sensor> sensorOpslag;  
    std::vector<actuator> actuatoren;
    std::vector<uint8_t> responseBuffer;
		
    int fifoReadFd, fifoWriteFd, teller;

    void setActuators();
    void processSensorUpdates(const char* sensor, double inputWaarde);
    	   
    void processWemosGetCommand(const char* data);
    wemosStatuscontrole::actuator* returnActuator(const std::string& zoekNaam);
    wemosStatuscontrole::sensor* returnSensor(const std::string& zoekNaam);

};

#endif // STATUSCONTROL_H
