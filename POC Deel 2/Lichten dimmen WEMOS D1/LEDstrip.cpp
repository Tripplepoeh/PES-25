#include "LEDstrip.h"
#include <FastLED.h>
#include <Arduino.h>

// Definitie van enkele constante parameters
#define BRIGHTNESS 155  // Maximale helderheid van de LED-strip
#define LED_TYPE WS2811  // Type LED-strip
#define COLOR_ORDER GRB  // Kleurvolgorde van de LED-strip
#define LED_PIN D0       // Pin waarop de LED-strip is aangesloten
#define NUM_LEDS 9       // Aantal LEDs in de LED-strip

// Constructor van de BedLightController-klasse
// Initialiseert de LED-strip, druksensor en andere variabelen.
BedLightController::BedLightController(uint8_t ledPin, uint16_t numLeds, uint8_t sensorPin)
    : _ledPin(ledPin), _numLeds(numLeds), _druksensor(sensorPin),
      _waarde(100), _bedDetectedTime(0), _isDimming(false), _lightsOff(false) {
    _leds = new CRGB[_numLeds]; // Dynamisch geheugen toewijzen voor de LED-array
}

// Initialiseer de hardware en start de seriële communicatie
void BedLightController::begin() {
    Serial.begin(115200); // Start seriële communicatie voor debugging
    ledsinit();           // Initialiseer de LED-strip
    _druksensor.begin();  // Initialiseer de druksensor
}

// Update de status van de LED-strip en druksensor
void BedLightController::update() {
    if (_lightsOff) {
        // Als de lichten uit zijn, doe niets
        return;
    }

    if (_druksensor.isPersonDetected()) {
        // Als de druksensor een persoon detecteert
        if (!_isDimming) {
            _isDimming = true;
            _bedDetectedTime = millis(); // Start de timer bij detectie
        }
        
        // Dim de lichten zolang de sensor is ingedrukt
        dimLights();

        // Zet de lichten uit als de sensor 10 seconden is ingedrukt
        if (millis() - _bedDetectedTime >= 10000UL) {
            turnOffLights();
        }
    } else {
        // Herstel wit licht als de sensor wordt losgelaten
        if (_isDimming) {
            _isDimming = false;
        }
        restoreWhiteLight();
    }

    // Voeg een kleine vertraging toe voor een vloeiende update
    FastLED.delay(10);
}

// Initialiseer de LED-strip
void BedLightController::ledsinit() {
    pinMode(_ledPin, OUTPUT);       // Stel de LED-pin in als uitgang
    digitalWrite(_ledPin, HIGH);   // Zet de pin hoog als standaardwaarde

    delay(3000); // Vertraging voor stabilisatie
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(_leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS); // Stel de helderheid in

    // Start met de LED-strip in wit licht
    for (int i = 0; i < _numLeds; i++) {
        _leds[i] = CRGB::White;
    }
    FastLED.show();
}

// Zet alle lichten uit door ze zwart te maken
void BedLightController::turnOffLights() {
    for (int i = 0; i < _numLeds; i++) {
        _leds[i] = CRGB::Black;
    }
    FastLED.show();
    _lightsOff = true; // Markeer dat de lichten uit zijn
}

// Dim de lichten geleidelijk
void BedLightController::dimLights() {
    static uint8_t currentBrightness = BRIGHTNESS;

    if (currentBrightness > 10) {  // Zorg dat de helderheid niet volledig naar 0 gaat
        currentBrightness -= 2; // Stel de dimsnelheid in
        FastLED.setBrightness(currentBrightness);
        FastLED.show();
    }
}

// Herstel het witte licht met een geleidelijke overgang
void BedLightController::restoreWhiteLight() {
    static uint8_t currentBrightness = 0;

    if (currentBrightness < BRIGHTNESS) {
        currentBrightness += 5; // Stel de herstelsnelheid in
        if (currentBrightness > BRIGHTNESS) currentBrightness = BRIGHTNESS;
        FastLED.setBrightness(currentBrightness);

        // Zet alle LEDs terug naar wit
        for (int i = 0; i < _numLeds; i++) {
            _leds[i] = CRGB::White;
        }
        FastLED.show();
    }
}
