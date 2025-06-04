#ifndef RFID_SENSOR_HPP
#define RFID_SENSOR_HPP

#include <SPI.h>
#include <MFRC522.h>

class RFIDSensor {
public:
    RFIDSensor(uint8_t ssPin, uint8_t rstPin);
    void init();
    bool checkCard();
    uint8_t* getUID();
    uint8_t getUIDSize();

private:
    MFRC522 mfrc522;
    uint8_t uid[5];
    uint8_t uidBuffer[10];   
    uint8_t uidSize;
};

#endif
