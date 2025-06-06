#include "RFIDSensor.hpp"

RFIDSensor::RFIDSensor(uint8_t ssPin, uint8_t rstPin)
  : mfrc522(ssPin, rstPin) {}

void RFIDSensor::init() {
  if (teller == 0) {
    SPI.begin();
  }
    mfrc522.PCD_Init();       
    delay(10); 
    teller++;        
}


bool RFIDSensor::checkKaart() {
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
        return false;
    }
    
    for (byte i = 0; i < 5; ++i) {
        uid[i] = mfrc522.uid.uidByte[i];
    }

    mfrc522.PICC_HaltA(); 
    return true;
}

uint8_t* RFIDSensor::getUID() {
    return uid;
}



