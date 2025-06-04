#include "RFIDSensor.hpp"

RFIDSensor::RFIDSensor(uint8_t ssPin, uint8_t rstPin)
  : mfrc522(ssPin, rstPin) {}

void RFIDSensor::init() {
    SPI.begin();              // Init SPI bus
    mfrc522.PCD_Init();       // Init MFRC522
    delay(10);                // Stabilize
}

bool RFIDSensor::checkCard() {
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
        return false;
    }

    // Copy UID bytes
    for (byte i = 0; i < 5; ++i) {
        uid[i] = mfrc522.uid.uidByte[i];
    }

    mfrc522.PICC_HaltA();  // Halt communication
    return true;
}

uint8_t* RFIDSensor::getUID() {
    return uid;
}


// uint8_t RFIDSensor::getUIDSize() {
//     return uidSize;
// }
