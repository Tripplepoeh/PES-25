/*
 * RFIDSensor.hpp
 *
 *  Created on: Apr 25, 2025
 *      Author: aashi
 */

#ifndef INC_RFIDSENSOR_HPP_
#define INC_RFIDSENSOR_HPP_

#include <stdio.h>
#include <stdint.h>

//dit zorgt ervoor dat de compiler de functies van rc522 niet mangled en dat de functies bruikbaar blijven zoals ze heten
#ifdef __cplusplus
extern "C" {
#endif
#include "RC522.h"    // C-header met MFRC522_Init, MFRC522_Request, etc.
#ifdef __cplusplus
}
#endif

class RFIDSensor {
private:
    uint8_t status;
    uint8_t opslagRauweWaardes[16];
    uint8_t RFIDWaarde[5];
    uint8_t toegangsKaarten[2][5]; //opslag kaart waardes die toegang mogen hebben
    uint8_t aantalKaarten;

public:
    RFIDSensor(); // constructor om de sensor te initialiseren
    ~RFIDSensor(); // destructor

    // Functies
    void RFIDSensorInit();  // Initialisatie van de rfidsensor
    int CheckKaart();       // Functie die checkt of er een kaart is
    uint8_t* RFIDSensorWaarde(); // Functie die de rfidsensor waardes omzet in 5 getallen
    int CheckToegang();     // Functie die checkt of de kaart toegang mag hebben
};




#endif /* INC_RFIDSENSOR_HPP_ */
