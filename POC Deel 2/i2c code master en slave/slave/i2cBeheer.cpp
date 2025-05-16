/*
 * i2cBeheer.cpp
 *
 *  Created on: May 11, 2025
 *      Author: aashi
 */

#include "i2cBeheer.hpp"
#include "alleDefines.h"
#include <iostream>
#include "lamp.h"
#include "noodknop.h"
#include <cstring>



I2CBeheer::I2CBeheer() : txLength(0), geelLamp(nullptr), noodk(nullptr) {
}

void I2CBeheer::I2CInit(std::vector<uint8_t> Ids, lamp* gl, noodknop* nk) {
    sensorIds = Ids;
    txLength = 0;
    actuatorIds = {LEDSTRIP, DEUR, DEURSERVO, BUZZER, LICHTKRANT,
                   SPECIALBEHEERDISPLAY, ROODLAMP, GROENLAMP, GEELLAMP};
    geelLamp = gl;
    noodk = nk;

    geelLamp->zetAan();
    HAL_Delay(1000);
    geelLamp->zetUit();
}

//void I2CBeheer::I2CInit(std::vector<uint8_t> Ids, RFIDSensor *rf, deurknop *kn, deurServo *ser){
//	    sensorIds = Ids;
//	    txLength = 0;
//	    actuatorIds = {LEDSTRIP, DEUR, DEURSERVO, BUZZER, LICHTKRANT,
//	                   SPECIALBEHEERDISPLAY, ROODLAMP, GROENLAMP, GEELLAMP};
//	    rfid = rf;
//	    knop = kn;
//	    servo = ser;
//}

void I2CBeheer::ProcessReceivedData(uint8_t *data, uint16_t size) {
    if (size >= 2 && data[0] == GET_WAARDE && data[1] == SET_ACTUATOR) {
        setBerichtKlaar();
        return;
    } else if (size >= 3 && data[0] == SET_ACTUATOR) {
        voerUit(data, size);
        return;
    } else if (size == 2) {
        uint8_t id = data[0];
        uint8_t waarde = data[1];

        // Vorm tijdelijk SET_ACTUATOR-stijl bericht om bestaande logica te hergebruiken
        uint8_t nieuwData[3] = {SET_ACTUATOR, id, waarde};
        voerUit(nieuwData, 3);
        return;
    }

    std::cerr << "Ongeldig I2C-commando ontvangen.\n";
}


void I2CBeheer::setBerichtKlaar() {
    txLength = 0;

    for (uint8_t id : sensorIds) {
        if (txLength + 2 > sizeof(txBericht)) break;

        if (actuatorIds.count(id)) {
            txBericht[txLength++] = GET_WAARDE;
            txBericht[txLength++] = id;
            continue;
        }

        txBericht[txLength++] = id;

        uint8_t waardeBytes[5] = {0};
        int bytesToWrite = (id == RFIDSENSOR) ? 5 : 2;

        switch (id) {
//            case DEURKNOP: {
//                uint16_t val = knop ? knop->statusopen() : 0;
//                waardeBytes[0] = val & 0xFF;
//                waardeBytes[1] = (val >> 8) & 0xFF;
//                break;
//            }
//            case RFIDSENSOR: {
//                if (rfid) {
//                    uint8_t* kaart = rfid->CheckKaart();
//                    if (kaart) {
//                        memcpy(waardeBytes, kaart, 5);
//                    }
//                }
//
//                break;
//            }
            case NOODKNOP: {
            	uint32_t val = noodk->getWaarde();
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


void I2CBeheer::voerUit(uint8_t *data, uint16_t size) {
    size_t index = 0;

    while (index + 2 < size && data[index] == SET_ACTUATOR) {
        uint8_t id = data[index + 1];
        uint8_t waarde = data[index + 2];

        switch (id) {
//            case DEUR: {
//                if (servo) {
//                    waarde ? servo->zetAan() : servo->zetUit();
//                }
//                break;
//            }
        case GEELLAMP: {
        	 waarde ? geelLamp->zetAan() : geelLamp->zetUit();
                    	break;
                               }
            default:
                std::cerr << "Onbekende actuator ID: " << std::hex << (int)id << "\n";
                break;
        }

        index += 3;
    }
}

const uint8_t* I2CBeheer::getBericht() const {
    return txBericht;
}

uint8_t I2CBeheer::getBerichtLength() const {
    return txLength;
}
