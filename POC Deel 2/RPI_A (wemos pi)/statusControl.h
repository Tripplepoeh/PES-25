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
#include <chrono>

// pad naar fifo bestanden
#define FIFO_READ "/tmp/pi_to_wemosserver"
#define FIFO_WRITE "/tmp/wemosserver_to_pi"

class statuscontrole {
public:
    // lijst met beschikbare sensoren
    enum Sensoren {
        deurknop = 0,   // sensor 0
        noodknop,       // sensor 1
        buzzerknop,     // sensor 2
        druksensor,     // sensor 3
        grondsensor, tempsensor, rfidsensor, co2sensor, bewegingsensor,
        aantalSensoren // totaal aantal sensoren
    };

    // namen van de sensoren
    const char* sensorNamen[aantalSensoren] = {
        "deurknop", "noodknop", "buzzerknop", "druksensor",
        "grondsensor", "tempsensor", "rfidsensor", "co2sensor", "bewegingsensor"
    };

    // lijst met beschikbare actuatoren
    enum Actuatoren {
        LEDSTRIP = 0, DEUR, DEURSERVO, BUZZER, LICHTKRANT,
        SPECIALBEHEERDISPLAY, ROODLAMP, GROENLAMP, GEELLAMP,
        aantalActuatoren
    };

    // namen van de actuatoren in dezelfde volgorde als de enum
    const char* actuatorNames[aantalActuatoren] = {
        "ledstrip", "deur", "deurservo", "buzzer", "lichtkrant",
        "specialbeheerdisplay", "roodlamp", "groenlamp", "geellamp"
    };

    statuscontrole();                   
    ~statuscontrole();                  

    void statuscontroleInit();         // initialiseer fifo verbindingen
    char* parseInput(const char* input);   // verwerk input van client
    int getWaarde(const char* sensor);     // geef sensorwaarde terug
    void checkFifo();                      // lees en verwerk fifo data
    void stuurNaarFifo(const char* message); // stuur bericht naar pi via fifo
    void setSensorWaarden(const char* sensor, uint64_t waarde); // zet specifieke sensorwaarde
    int getActuatorState(const char* naam); // geef status van actuator
    void updateActuatorStates();           // update status van alle actuatoren

private:
    uint64_t sensorWaarden[aantalSensoren];           // huidige waarden van sensoren
    int vorigeSensorWaarden[aantalSensoren];     // vorige waarden van sensoren
    uint64_t actuatorStates[aantalActuatoren]{};  // status van actuatoren
    char sendBuffer[512];                        // buffer voor fifo berichten

    int fifoReadFd = -1;   // file descriptor voor lezen uit fifo
    int fifoWriteFd = -1;  // file descriptor voor schrijven naar fifo
};

#endif // STATUSCONTROLE_H
