#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <Arduino.h>
#include <FastLED.h>

class LEDstrip {
public:
    LEDstrip(uint8_t pin, uint16_t numLeds);
    void init();
    void setColor(int situation);
    void update();
    void lichtUit();
    void lichtAan();

private:
    uint8_t _pin;
    uint16_t _numLeds;
    CRGB* _leds;
    int _currentState;
    int r, g, b = 0;
    
};

#endif // LEDSTRIP_H
