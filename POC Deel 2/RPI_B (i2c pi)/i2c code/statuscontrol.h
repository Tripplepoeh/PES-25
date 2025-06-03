// statuscontrol.h
#ifndef STATUSCONTROL_H
#define STATUSCONTROL_H

#include <vector>
#include <chrono>
#include <string>
#include <cstdint>

#include "alleDefines.h"

#define FIFO_READ  "/tmp/pi_to_i2cmaster"
#define FIFO_WRITE "/tmp/i2cmaster_to_pi"

class logToDatabase;
class getFromDatabase;


class statuscontrole {
public:
    statuscontrole(logToDatabase* log, getFromDatabase* get);

    const std::vector<uint8_t>& getResponses() const;
    void clearResponses();
    void processI2CData(const uint8_t* data, size_t length);
    void handleFifoRead();
    void sendMessageToFifo();

private:
    std::vector<uint8_t> sensorIds, actuatorStates, responseBuffer;
    std::vector<std::string> sensorNames;
    std::vector<double> sensorWaarden, vorigeSensorWaarden;
    std::chrono::steady_clock::time_point lichtkrantStartTijd;
    uint8_t lichtkrantStatus = 0;
    int fifoReadFd=-1, fifoWriteFd=-1, teller = 0;

    void setActuators();
    void processSensorUpdates(const uint8_t* data, size_t& i, size_t len);
    void processI2CCommands(const uint8_t* data, size_t& i, size_t len);
    logToDatabase* logger;
    getFromDatabase* getDatabase;
};

#endif // STATUSCONTROL_H
