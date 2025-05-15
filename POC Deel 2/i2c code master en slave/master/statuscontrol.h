#ifndef STATUSCONTROL_H
#define STATUSCONTROL_H

#include <vector>
#include <chrono>
#include <string>
#include <cstdint>

#include "alleDefines.h"

// definieer de paden voor fifo-bestanden voor communicatie
#define FIFO_READ  "/tmp/pi_to_i2cmaster"
#define FIFO_WRITE "/tmp/i2cmaster_to_pi"

class statuscontrole {
public:
    statuscontrole();  // constructor

    const std::vector<uint8_t>& getResponses() const; // geef de responsebuffer terug
    void clearResponses(); // maak de responsebuffer leeg
    void processI2CData(const uint8_t* data, size_t length); // verwerk i2c-data
    void addActuatorResponse(uint8_t id, uint8_t val); // voeg actuatorrespons toe
    void handleFifoRead(); // lees opdrachten vanuit fifo-bestand
    void sendMessageToFifo(); // verstuur sensorveranderingen naar fifo

private:
    std::vector<uint8_t> sensorIds;              // lijst met sensor-ids
    std::vector<std::string> sensorNames;        // overeenkomstige namen
    std::vector<uint32_t> sensorWaarden, vorigeSensorWaarden; // huidige en vorige waarden
    std::vector<uint8_t> responseBuffer;         // buffer voor antwoorden aan master
    std::vector<uint8_t> actuatorStates;         // status per actuator-id (index is id)
    std::chrono::steady_clock::time_point lichtkrantStartTijd; // starttijd voor lichtkrant
    uint8_t lichtkrantStatus = 0;                // huidige status lichtkrant
    int fifoReadFd=-1, fifoWriteFd=-1;           // filedescriptors voor fifo-bestanden

    void recomputeActuators(); // bepaal nieuwe actuatorwaarden o.b.v. sensorwaarden
    void processSensorUpdates(const uint8_t* data, size_t& i, size_t len); // verwerk binnenkomende sensorwaarden
    void processI2CCommands(const uint8_t* data, size_t& i, size_t len);   // verwerk i2c-opdrachten
};

#endif // STATUSCONTROL_H
