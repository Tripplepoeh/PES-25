#ifndef STATUSCONTROL_H
#define STATUSCONTROL_H

#include <vector>
#include <unordered_map>
#include <chrono>
#include <string>

// Include de definities van alle sensor- en actuator-IDs
#include "alleDefines.h"

class statuscontrole {
public:
    statuscontrole();

    const std::vector<uint8_t>& getResponses() const;        // Haal de response buffer op
    void clearResponses();                                   // Leeg de response buffer
    void processI2CData(const uint8_t* data, size_t length);  // Verwerk inkomende I2C data
    //void generateActuatorCommands(std::vector<uint8_t>& cmds); // Genereer actuatorcommando's
    void addActuatorResponse(uint8_t id, uint8_t val);         // Voeg actuator respons toe aan buffer

private:
    // Hulpvariabelen
    std::vector<uint8_t> sensorIds;                          // Lijst van sensor-ID's
    std::vector<std::string> sensorNames;                     // Lijst van sensor namen
    std::vector<uint32_t> sensorWaarden;                      // Lijst van sensor waarden
    std::vector<uint32_t> vorigeSensorWaarden;                // Lijst van vorige sensorwaarden
    std::unordered_map<uint8_t, uint8_t> actuatorStates;      // Lijst van actuatorstatussen
    std::unordered_map<uint8_t, std::string> actuatorNames;   // Lijst van actuator namen
    std::chrono::steady_clock::time_point lichtkrantStartTijd; // Tijdstip van de start van de lichtkrant
    uint8_t lichtkrantStatus;                                // Status van de lichtkrant

    std::vector<uint8_t> responseBuffer;                      // Buffer voor de response

    // Hulpfuncties
    int getSensorIndexById(uint8_t id) const;                 // Zoek sensor op basis van ID
    size_t getSensorDataLength(uint8_t id) const;              // Bepaal de hoeveelheid data voor sensor
    void processSensorUpdates(const uint8_t* data, size_t& i, size_t length); // Verwerk sensor updates
    uint32_t combineBytesToValue(const uint8_t* bytes, size_t length);  // Converteer bytes naar waarde
    void processI2CCommands(const uint8_t* data, size_t& i, size_t length); // Verwerk I2C commando's
    void recomputeActuators();                                 // Herbereken actuatorstatus
};

#endif
