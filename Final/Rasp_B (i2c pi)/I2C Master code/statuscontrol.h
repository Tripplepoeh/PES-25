
#ifndef STATUSCONTROL_H
#define STATUSCONTROL_H
#include "istatuscontrole.h"



//#include "alleDefines.h"

#define FIFO_READ  "/tmp/pi_to_i2cmaster"
#define FIFO_WRITE "/tmp/i2cmaster_to_pi"

class logToDatabase;
class getFromDatabase;



class statuscontrole : public istatuscontrole {
public:
	statuscontrole(logToDatabase* log, getFromDatabase* get);

	void procesData(const uint8_t* data, size_t length);
	void leesFifo();
	void stuurBerichtFifo();
	const std::vector<uint8_t>& getResponse() const;

private:
	struct sensor {
		uint8_t sensorId;
		std::string sensorNaam;
		double sensorWaarde;
		double vorigeSensorWaarde;
        uint64_t rawWaarde;
	};

	struct actuator {
		uint8_t actuatorId;
		std::string actuatorNaam;
		uint8_t actuatorStatus;
	};

	std::vector<sensor> sensorOpslag;
	std::vector<actuator> actuatoren;
    std::vector<uint8_t> responseBuffer;
    

	int fifoReadFd, fifoWriteFd, teller;

	void setActuators();
	void processSensorUpdates(const uint8_t* data, size_t& i, size_t len);
	void processI2CCommands(const uint8_t* data, size_t& i, size_t len);
    statuscontrole::actuator* returnActuator(const  uint8_t& id);
    statuscontrole::sensor* returnSensor(const  uint8_t& id);
    
	logToDatabase* logger;
	getFromDatabase* getDatabase;
};

#endif // STATUSCONTROL_H
