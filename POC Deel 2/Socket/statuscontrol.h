#ifndef STATUSCONTROLE_H
#define STATUSCONTROLE_H

#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>
#include <sstream>
#include <algorithm>

#define FIFO_READ "/tmp/pi_to_wemosserver"
#define FIFO_WRITE "/tmp/wemosserver_to_pi"

class statuscontrole {
private:
    // Set waardes
    int sensorWaarden[4];  
    int vorigeSensorWaarden[4]; 
    char sendBuffer[512];

    int fifoReadFd = -1;
    int fifoWriteFd = -1;

public:
    enum Sensoren {
        drukWaarde = 0,   // Sensor 0
        noodknopWaarde,         // Sensor 1
        plantWaarde,      // Sensor 2
        bewegingsWaarde,   // Sensor 3
        aantalSensoren   // Houdt het aantal sensoren bij
    };
    
   const char* sensorNamen[aantalSensoren] = {"druksensor", "noodknop", "plant", "beweging"};

    statuscontrole();
    ~statuscontrole();
	
	void statuscontroleInit();
    char* parseInput(const char* input);
    int getWaarde(const char* sensor);
    void checkFifo();
    void stuurNaarFifo(const char* message);
    void setSensorWaarden(const char* sensor, int waarde);
};

#endif // STATUSCONTROLE_H
