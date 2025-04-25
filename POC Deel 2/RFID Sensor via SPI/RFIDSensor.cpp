/*
 * RFIDSensor.cpp
 *
 *  Created on: Apr 25, 2025
 *      Author: aashi
 */


#include "RFIDSensor.hpp"
#include "RC522.h"  // Library die nodig is om de RFID sensor uit te lezen
#include <string.h>

// Constructor
RFIDSensor::RFIDSensor() : aantalKaarten(0) {
    // Kan de sensor mogelijk initialiseren hier als je wilt
    RFIDSensorInit();
}

// Destructor
RFIDSensor::~RFIDSensor() {
    // Eventuele opruimacties kunnen hier worden toegevoegd
}

// Initialisatie van de RFIDSensor
void RFIDSensor::RFIDSensorInit() {
    MFRC522_Init();  // Initializeer de MFRC522 module
}

// Functie die checkt of er een kaart is
int RFIDSensor::CheckKaart() {
    // Dit retourneert MI_OK als de RFIDSensor een kaart/tag heeft gelezen
    status = MFRC522_Request(PICC_REQIDL, opslagRauweWaardes);
    if (status == MI_OK) {
        // Dit retourneert of de sensor de waardes van de tag/kaart heeft kunnen halen
        status = MFRC522_Anticoll(opslagRauweWaardes);
        if (status == MI_OK) {
            // Als de RFID sensor alle waardes heeft kunnen ophalen en sturen geeft hij een true door als return
            return 1;
        }
    }
    return 0;
}

// Functie die de RFIDSensor waardes omzet in 5 getallen in een string/char array
uint8_t* RFIDSensor::RFIDSensorWaarde() {
    // Dit zet de juiste waardes van opslagRauweWaardes naar RFIDWaarde, wat de waarde van de tag/kaart is
    memcpy(RFIDWaarde, opslagRauweWaardes, sizeof(uint8_t) * 5);
    return RFIDWaarde;
}

// Deze functie vergelijkt of er toegang verleend mag worden aan een kaart
int RFIDSensor::CheckToegang() {
    // Ga langs alle kaarten en check of de gescande kaart overeenkomt met een kaart die toegang mag hebben
    for (int i = 0; i < aantalKaarten; i++) {
        if (memcmp(RFIDWaarde, toegangsKaarten[i], 5) == 0) return 1; // Als de kaart overeenkomt, toegang geven
    }

    // Check of er al 2 kaarten gescand zijn, zo niet voeg de kaart toe als toegangskaart
    if (aantalKaarten < 2) {
        memcpy(toegangsKaarten[aantalKaarten], RFIDWaarde, 5);
        aantalKaarten++;
        return 1;  // Toegang verlenen
    }

    return 0;  // Geen toegang, kaarten zijn vol
}



