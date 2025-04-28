#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <Arduino.h>
#include <FastLED.h>

// Definitie van de LEDstrip-klasse
class LEDstrip {
public:
    // Constructor: initialiseert de LED-strip met de opgegeven pin en aantal LEDs
    LEDstrip(uint8_t pin, uint16_t numLeds);

    // Initialiseer de LED-strip (hardware en bibliotheek-instellingen)
    void init();

    // Stel de kleur van de LED-strip in afhankelijk van een specifieke situatie
    void setColor(int situation);

    // Update de LED-strip en toon de huidige status
    void update();

    // Schakel de LED-strip uit (fade-out effect)
    void lichtUit();

    // Schakel de LED-strip aan (fade-in effect)
    void lichtAan();

private:
    uint8_t _pin;         // De pin waarop de LED-strip is aangesloten
    uint16_t _numLeds;    // Het aantal LEDs in de LED-strip
    CRGB* _leds;          // Pointer naar een array van CRGB-objecten (voor kleurbeheer van de LEDs)
    int _currentState;    // Huidige status/situatie van de LED-strip
    int r, g, b = 0;      // RGB-kleurenwaarden voor de LEDs
};

#endif // LEDSTRIP_H
