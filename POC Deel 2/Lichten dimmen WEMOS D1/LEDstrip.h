#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <FastLED.h>       // Bibliotheek voor het aansturen van LED-strips
#include <Arduino.h>       // Arduino-bibliotheek voor algemene functies
#include "Druksensor.h"    // Headerbestand voor de druksensorfunctionaliteit

// Klasse voor het beheren van een LED-strip in combinatie met een druksensor
class BedLightController {
public:
    // Constructor: Initialiseert de LED-strip en druksensor.
    // ledPin: De pin waarop de LED-strip is aangesloten.
    // numLeds: Het aantal LEDs in de LED-strip.
    // sensorPin: De pin waarop de druksensor is aangesloten.
    BedLightController(uint8_t ledPin, uint16_t numLeds, uint8_t sensorPin);

    // Initialiseer de hardware (LED-strip en druksensor).
    void begin();

    // Voer de logica van de LED-strip en druksensor uit.
    void update();

    // Initialiseer de LED-strip met standaard instellingen.
    void ledsinit();

    // Zet alle LEDs uit (zwart maken).
    void turnOffLights();

    // Dim de LEDs geleidelijk.
    void dimLights();

    // Herstel het witte licht van de LEDs.
    void restoreWhiteLight();

private:
    uint8_t _ledPin;               // Pin waarop de LED-strip is aangesloten
    uint16_t _numLeds;             // Aantal LEDs in de LED-strip
    Druksensor _druksensor;        // Object voor het beheren van de druksensor
    int _waarde;                   // Waarde voor extra functionaliteit (onbenut)
    unsigned long _bedDetectedTime; // Tijdstip waarop de druksensor werd geactiveerd
    bool _isDimming;               // Status om aan te geven of de LEDs aan het dimmen zijn
    bool _lightsOff;               // Status om aan te geven of de LEDs uit zijn
    CRGB* _leds;                   // Array van LEDs aangestuurd door de FastLED-bibliotheek
};

#endif // LEDSTRIP_H
