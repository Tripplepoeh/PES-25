// i2cBeheer.cpp
#include "i2cBeheer.hpp"
#include <iostream>
#include <cstring>
#include "geleLed.h"
#include "rodeLed.h"
#include "groeneLed.h"
//#include "deurServo.h"
//#include "deurknop.h"
#include "alleDefines.h"
extern "C" {
    #include "co2sensor_stm.h"
}
#include "bewegingsensor.h"
//#include "deurServoTest.h"

I2CBeheer::I2CBeheer()
    : txLength(0) {
}
/*Init functie die een vector verwacht met welke sensoren en actuatoren het bordje heeft
 * Er moeten ook adressen an de objecten gestuurd worden zodat deze bestuurd kunnen worden door I2CBeheer
 */
void I2CBeheer::I2CInit(std::vector<uint8_t> Ids, lamp* geel, lamp* groen, lamp* rood, Bewegingssensor* bws) {
    sensorIds = Ids;
    txLength = 0;
    //initialiseer actuatorIds
    actuatorIds = {LEDSTRIP, DEUR, DEURSERVO, BUZZER, LICHTKRANT,
                   SPECIALBEHEERDISPLAY, ROODLAMP, GROENLAMP, GEELLAMP};
    //koppel binnenkomedende adressen met in de header gedefinieerde objecten
    geelLamp = geel;
    roodLamp = rood;
    groenLamp = groen;
    bewegingsensor = bws;

}

void I2CBeheer::ProcessReceivedData(uint8_t* data, uint16_t size) {
	//Als master vraagt om sensorwaardes en actuatorbesturingen stuur dan alle sensorwaardes en welke actuator waarde je wilt ontvangen
    if (size >= 2 && data[0] == GET_WAARDE && data[1] == SET_ACTUATOR) {
        setBerichtKlaar();
    } else {
        voerUit(data, size); //Anders verwerk het binnenkomende data om actuators aan te st
    }
}

void I2CBeheer::voerUit(uint8_t* data, uint16_t size) {
    // Er moeten minstens 2 bytes zijn namelijk id en dan waarde
    if (size < 2) {
        return;
    }
    //loop door alle paren om hun waardes te krijgen
    for (uint16_t i = 0; i + 1 < size; i += 2) {
        uint8_t id    = data[i];
        uint8_t waarde = data[i + 1];

        switch (id) {
//            case DEUR:
//                if (servo) {
//                    waarde ? servo->zetAan() : servo->zetUit(); //Aan als waarde 1 if anders uit
//                }
//                break;
//
            case GROENLAMP:
                if (groenLamp) {
                    waarde ? groenLamp->zetAan() : groenLamp->zetUit();//Aan als waarde 1 if anders uit
                }
                break;
            case GEELLAMP:
                           if (geelLamp) {
                               waarde ? geelLamp->zetAan() : geelLamp->zetUit();//Aan als waarde 1 if anders uit
                           }
                           break;
            case ROODLAMP:
                           if (roodLamp) {
                               waarde ? roodLamp->zetAan() : roodLamp->zetUit();//Aan als waarde 1 if anders uit
                           }
                           break;


            // Voeg hier andere actuator cases toe en of vervang de andere cases
            default:
                std::cerr << "Onbekende actuator ID: 0x"
                          << std::hex << int(id) << "\n";
                break;
        }
    }
}


void I2CBeheer::setBerichtKlaar() {
    txLength = 0;
    //Ga langs alle ids in de vector die in de init is binnengekomen
    for (uint8_t id : sensorIds) {
        if (txLength + 2 > sizeof(txBericht)) break; //Check of er genoeg ruimte is in txbericht
        if (actuatorIds.count(id)) {
        	// Vraag de actuator state op via GET_WAARDE id
            txBericht[txLength++] = GET_WAARDE;
            txBericht[txLength++] = id;
        } else {
        	//Haal sensorwaardes op en zet ze in de buffer
            txBericht[txLength++] = id; //Zet de id van sensor in het bericht
            uint8_t waardeBytes[5] = {0}; //5 bytes om alle sensorwaardes te kunnen sturen
            int bytesToWrite = 2;
            switch (id) {
                case CO2SENSOR: {
                    uint16_t val = getCo2Waarde();
                    waardeBytes[0] = val & 0xFF; //Zet de waarde om in little endian en 2 bytes
                    waardeBytes[1] = (val >> 8) & 0xFF;
                    break;
                }
                case BEWEGINGSENSOR: {
                	uint16_t val = bewegingsensor ? bewegingsensor->GetWaarde() : 0;
                	                    waardeBytes[0] = val & 0xFF; //Zet de waarde om in little endian en 2 bytes
                	                    waardeBytes[1] = (val >> 8) & 0xFF;
                	                    break;
                                }
                default:
                    break;
            }
            //Zet de sensorwaardes in het bericht
            if (txLength + bytesToWrite > sizeof(txBericht)) break;
            for (int i = 0; i < bytesToWrite; ++i) {
                txBericht[txLength++] = waardeBytes[i];
            }
        }
    }
}
//functie die het bericht teruggeeft aan de i2c code
const uint8_t* I2CBeheer::getBericht() const {
    return txBericht;
}
//functie die de lengte van het bericht teruggeeft aan de i2c code
uint8_t I2CBeheer::getBerichtLength() const {
    return txLength;
}
