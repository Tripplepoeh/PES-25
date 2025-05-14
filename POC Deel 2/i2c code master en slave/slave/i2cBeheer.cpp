// i2cBeheer.cpp
#include "i2cBeheer.hpp"
#include <iostream>
#include <cstring>
#include "lamp.h"
#include "deurServo.h"
#include "deurknop.h"
#include "alleDefines.h"

I2CBeheer::I2CBeheer()
    : txLength(0) {
}

void I2CBeheer::I2CInit(std::vector<uint8_t> Ids, deurknop *kn, lamp* gl, deurServo *ser) {
    sensorIds = Ids;
    txLength = 0;
    actuatorIds = {LEDSTRIP, DEUR, DEURSERVO, BUZZER, LICHTKRANT,
                   SPECIALBEHEERDISPLAY, ROODLAMP, GROENLAMP, GEELLAMP};
    knop = kn;
    geelLamp = gl;
    servo = ser;
}

void I2CBeheer::ProcessReceivedData(uint8_t* data, uint16_t size) {
    if (size >= 2 && data[0] == GET_WAARDE && data[1] == SET_ACTUATOR) {
        setBerichtKlaar();
    } else {
        voerUit(data, size);
    }
}

void I2CBeheer::voerUit(uint8_t* data, uint16_t size) {
    size_t index = 0;
    // detect if prefixed with SET_ACTUATOR frames
    if (size >= 3 && data[0] == SET_ACTUATOR) {
        while (index + 2 < size) {
            if (data[index] == SET_ACTUATOR) {
                uint8_t id = data[index + 1];
                uint8_t waarde = data[index + 2];
                // stuur actuator aan
                switch (id) {
                    case DEUR:
                        if (servo) waarde ? servo->zetAan() : servo->zetUit();
                        break;
                    case GROENLAMP:
                        if (geelLamp) waarde ? geelLamp->zetAan() : geelLamp->zetUit();
                        break;
                    // voeg hier andere actuator cases toe
                    default:
                        std::cerr << "Onbekende actuator ID: 0x" << std::hex << int(id) << "\n";
                        break;
                }
                index += 3;
            } else {
                ++index; // skip unexpected byte
            }
        }
    } else if (size % 2 == 0) {
        // raw id,value pairs
        for (size_t i = 0; i + 1 < size; i += 2) {
            uint8_t id = data[i];
            uint8_t waarde = data[i + 1];
            switch (id) {
                case DEUR:
                    if (servo) waarde ? servo->zetAan() : servo->zetUit();
                    break;
                case GROENLAMP:
                    if (geelLamp) waarde ? geelLamp->zetAan() : geelLamp->zetUit();
                    break;
                // voeg hier andere actuator cases toe
                default:
                    std::cerr << "Onbekende actuator ID: 0x" << std::hex << int(id) << "\n";
                    break;
            }
        }
    } else {
        std::cerr << " Ongeldig actuator-commando formaat (size=" << size << ").\n";
    }
}

void I2CBeheer::setBerichtKlaar() {
    txLength = 0;
    for (uint8_t id : sensorIds) {
        if (txLength + 2 > sizeof(txBericht)) break;
        if (actuatorIds.count(id)) {
            txBericht[txLength++] = GET_WAARDE;
            txBericht[txLength++] = id;
        } else {
            txBericht[txLength++] = id;
            uint8_t waardeBytes[5] = {0};
            int bytesToWrite = 2;
            switch (id) {
                case DEURKNOP: {
                    uint16_t val = knop ? knop->isOpen() : 0;
                    waardeBytes[0] = val & 0xFF;
                    waardeBytes[1] = (val >> 8) & 0xFF;
                    break;
                }
                default:
                    break;
            }
            if (txLength + bytesToWrite > sizeof(txBericht)) break;
            for (int i = 0; i < bytesToWrite; ++i) {
                txBericht[txLength++] = waardeBytes[i];
            }
        }
    }
}

const uint8_t* I2CBeheer::getBericht() const {
    return txBericht;
}

uint8_t I2CBeheer::getBerichtLength() const {
    return txLength;
}
