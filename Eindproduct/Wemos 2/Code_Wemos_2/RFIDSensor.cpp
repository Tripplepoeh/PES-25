#include "RFIDSensor.hpp"

RFIDSensor::RFIDSensor(uint8_t ssPin, uint8_t rstPin)
  : mfrc522(ssPin, rstPin) {}

/*! initialisatie van de sensor, de eerste keer dat deze aangaat wordt SPI ook gestart. de keren daarna wordt SPI niet opnieuw opgestart. 
dit wordt gedaan omdat de sensor na inactiviteit wordt heropgestart, dit is echter onnodig voor de SPI */
void RFIDSensor::init() {
  if (teller == 0) {
    SPI.begin();
  }
    mfrc522.PCD_Init();       
    delay(10); 
    teller++;        
}

/*! check of er een kaart woordt gezien */
bool RFIDSensor::checkKaart() {
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) { //! check of er een leesbare kaart te zien is. zo niet, return false.
        return false;
    }
     /*! zet de uidbytes in de array */
    for (byte i = 0; i < 5; ++i) {
        uid[i] = mfrc522.uid.uidByte[i];
    }

    mfrc522.PICC_HaltA(); //! stop communicatie met de kaart
    return true;
}

uint8_t* RFIDSensor::getUID() {
    return uid;
}



