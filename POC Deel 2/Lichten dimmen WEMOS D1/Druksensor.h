#ifndef DRUKSENSOR_H
#define DRUKSENSOR_H

#include <Arduino.h>

class Druksensor {
public:
    Druksensor(uint8_t sensorPin);  
    void begin();                   
    bool isPersonDetected();         

private:
    uint8_t _sensorPin;              
};

#endif // DRUKSENSOR_H
