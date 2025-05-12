#include "statuscontrol.h"
#include <iostream>
#include <chrono>

statuscontrole::statuscontrole() {
    sensorIds   = { DEURKNOP, NOODKNOP, BUZZERKNOP, DRUKSENSOR, GRONDSENSOR, TEMPSENSOR, RFIDSENSOR, CO2SENSOR, BEWEGINGSENSOR };
    sensorNames = { "deurknop", "noodknop", "buzzerknop", "druksensor", "grondsensor", "tempsensor", "rfidsensor", "co2sensor", "bewegingsensor" };
    sensorWaarden.assign(sensorIds.size(), 0);
    vorigeSensorWaarden.assign(sensorIds.size(), 0);

    actuatorNames = {
        { LEDSTRIP, "ledstrip" },{ DEUR,     "deur" },{ DEURSERVO,"deurservo" },{ BUZZER,   "buzzer" },{ LICHTKRANT,   "lichtkrant" },
        { SPECIALBEHEERDISPLAY, "specialbeheerDisplay" },{ ROODLAMP, "roodlamp" },{ GROENLAMP,"groenlamp" },{ GEELLAMP, "geellamp" }
    };
    for (auto &p : actuatorNames) {
        actuatorStates[p.first] = GA_UIT;
    }

    lichtkrantStartTijd = std::chrono::steady_clock::now();
    lichtkrantStatus = 0;
}

const std::vector<uint8_t>& statuscontrole::getResponses() const {
    return responseBuffer;
}

void statuscontrole::clearResponses() {
    responseBuffer.clear();
}

int statuscontrole::getSensorIndexById(uint8_t id) const {
    for (size_t i = 0; i < sensorIds.size(); ++i) {
        if (sensorIds[i] == id) return i;
    }
    return -1;
}

size_t statuscontrole::getSensorDataLength(uint8_t id) const {
    if (id == RFIDSENSOR)
        return 5;
    return 2;
}

void statuscontrole::addActuatorResponse(uint8_t id, uint8_t val) {
    responseBuffer.push_back(id);
    responseBuffer.push_back(val);
}

void statuscontrole::recomputeActuators() {
    if (sensorWaarden[getSensorIndexById(NOODKNOP)] == 1) {
        actuatorStates[GEELLAMP] = GA_AAN;
        actuatorStates[BUZZER]   = GA_AAN;
    }

    actuatorStates[ROODLAMP] = (sensorWaarden[getSensorIndexById(CO2SENSOR)] > 800) ? GA_AAN : GA_UIT;
    actuatorStates[GROENLAMP] = (sensorWaarden[getSensorIndexById(GRONDSENSOR)] < 500) ? GA_AAN : GA_UIT;
    actuatorStates[DEURSERVO] = (sensorWaarden[getSensorIndexById(DEURKNOP)] == 1) ? GA_AAN : GA_UIT;

    // Lichtkrant toggle logica (tussen 0x70 en 0x71)
    auto nu = std::chrono::steady_clock::now();
    auto verstreken = std::chrono::duration_cast<std::chrono::seconds>(nu - lichtkrantStartTijd).count();
    if (verstreken >= 20) {
        lichtkrantStatus ^= 1; // toggle tussen 0 en 1
        lichtkrantStartTijd = nu;
    }
    actuatorStates[LICHTKRANT] = 0x70 + lichtkrantStatus;

    uint64_t code = sensorWaarden[getSensorIndexById(RFIDSENSOR)];
if (code != 0) {
    if (code == 0xB6C7283364 || code == 0x9120071DAB) {
        actuatorStates[DEUR] = GA_AAN;
    } else {
        actuatorStates[DEUR] = GA_UIT;
    }
}


    actuatorStates[LEDSTRIP] = (sensorWaarden[getSensorIndexById(BEWEGINGSENSOR)] == 0) ? GA_AAN : GA_UIT;
    
    actuatorStates[DEUR] = (sensorWaarden[getSensorIndexById(DEURKNOP)] == 1) ? GA_AAN : GA_UIT;
    // SPECIALBEHEERDISPLAY = CO2SENSOR-waarde (16-bit)
    actuatorStates[SPECIALBEHEERDISPLAY] = static_cast<uint8_t>(sensorWaarden[getSensorIndexById(CO2SENSOR)] & 0xFF); // alleen voor compatibiliteit
}

void statuscontrole::processSensorUpdates(const uint8_t* data, size_t& i, size_t length) {
    uint8_t id = data[i++];
    size_t dataLen = getSensorDataLength(id);

    if (i + dataLen > length) {
        std::cerr << "Ongeldige data: te weinig bytes voor sensor ID " << std::hex << (int)id << "\n";
        return;
    }

    uint32_t waarde = 0;
    for (size_t b = 0; b < dataLen; ++b) {
        waarde |= static_cast<uint32_t>(data[i + b]) << (8 * b);  // little endian
    }

    int sidx = getSensorIndexById(id);
    if (sidx >= 0) {
        vorigeSensorWaarden[sidx] = sensorWaarden[sidx];
        sensorWaarden[sidx] = waarde;
        std::cout << "Sensor " << sensorNames[sidx] << " updated to " << waarde << "\n";
    }

    i += dataLen;
}

void statuscontrole::processI2CCommands(const uint8_t* data, size_t& i, size_t length) {
    if (data[i] == GET_WAARDE) {
        i++;
        if (i < length) {
            uint8_t actuatorId = data[i++];
            if (actuatorId == SPECIALBEHEERDISPLAY) {
                uint16_t waarde = static_cast<uint16_t>(sensorWaarden[getSensorIndexById(CO2SENSOR)]);
                responseBuffer.push_back(SPECIALBEHEERDISPLAY);
                responseBuffer.push_back(waarde & 0xFF);
                responseBuffer.push_back((waarde >> 8) & 0xFF);
            } else {
                auto it = actuatorStates.find(actuatorId);
                if (it != actuatorStates.end()) {
                    responseBuffer.push_back(actuatorId);
                    responseBuffer.push_back(it->second);
                }
            }
        }
    } else if (data[i] == SET_ACTUATOR) {
        if (i + 2 < length) {
            uint8_t id = data[i + 1];
            uint8_t val = data[i + 2];
            actuatorStates[id] = val;
            i += 3;
        } else {
            i = length;  // Stop bij ongeldige invoer
        }
    } else {
        i++;
    }
}
void statuscontrole::processI2CData(const uint8_t* data, size_t length) {
    clearResponses();
    if (length < 2) return;

    size_t i = 0;

    // Verwerk sensor updates
    while (i < length && data[i] != GET_WAARDE && data[i] != SET_ACTUATOR) {
        processSensorUpdates(data, i, length);
    }

    recomputeActuators();

    // Verwerk I2C-commando's
    while (i < length) {
        processI2CCommands(data, i, length);
    }
}


