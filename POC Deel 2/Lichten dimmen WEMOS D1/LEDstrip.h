#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <FastLED.h>
#include <Arduino.h>
#include "Druksensor.h"

class BedLightController {
public:
    BedLightController(uint8_t ledPin, uint16_t numLeds, uint8_t sensorPin);
    void begin();
    void update();
    void ledsinit();
    void turnOffLights();
    void dimLights();           
    void restoreWhiteLight();   

private:
    uint8_t _ledPin;
    uint16_t _numLeds;
    Druksensor _druksensor;
    int _waarde;
    unsigned long _bedDetectedTime;
    bool _isDimming;
    bool _lightsOff;
    CRGB* _leds;
};

#endif // LEDSTRIP_H
