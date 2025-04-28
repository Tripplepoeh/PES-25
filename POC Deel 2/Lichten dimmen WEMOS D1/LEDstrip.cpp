#include "LEDstrip.h"
#include <FastLED.h>
#include <Arduino.h>

#define BRIGHTNESS 155  // Max heldere wit
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define LED_PIN D0
#define NUM_LEDS 9

BedLightController::BedLightController(uint8_t ledPin, uint16_t numLeds, uint8_t sensorPin)
    : _ledPin(ledPin), _numLeds(numLeds), _druksensor(sensorPin),
      _waarde(100), _bedDetectedTime(0), _isDimming(false), _lightsOff(false) {
    _leds = new CRGB[_numLeds];
}

void BedLightController::begin() {
    Serial.begin(115200);
    ledsinit();
    _druksensor.begin();
}

void BedLightController::update() {
    if (_lightsOff) {
        return;
    }

    if (_druksensor.isPersonDetected()) {
        // Als sensor ingedrukt is, langzaam dimmen naar zwart
        if (!_isDimming) {
            _isDimming = true;
            _bedDetectedTime = millis(); // Start tijd bij eerste detectie
        }
        
        // Dim de lichten zolang de sensor ingedrukt is
        dimLights();

        // Als de sensor voor 10 seconden is ingedrukt, zet lichten uit
        if (millis() - _bedDetectedTime >= 10000UL) {
            turnOffLights();
        }
    } else {
        // Als sensor losgelaten wordt, terug naar wit licht
        if (_isDimming) {
            _isDimming = false;
        }
        restoreWhiteLight();
    }

    FastLED.delay(10);  // Kleine vertraging om update soepel te maken
}

// void BedLightController::pasLedAan(int situatie) {
//     // Niet meer nodig voor dimmen/herstellen -> wordt nu apart gedaan
// }

void BedLightController::ledsinit() {
    pinMode(_ledPin, OUTPUT);
    digitalWrite(_ledPin, HIGH);

    delay(3000);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(_leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    // Start initieel met wit licht
    for (int i = 0; i < _numLeds; i++) {
        _leds[i] = CRGB::White;
    }
    FastLED.show();
}

void BedLightController::turnOffLights() {
    for (int i = 0; i < _numLeds; i++) {
        _leds[i] = CRGB::Black;
    }
    FastLED.show();
    _lightsOff = true;
}

void BedLightController::dimLights() {
    static uint8_t currentBrightness = BRIGHTNESS;

    if (currentBrightness > 10) {  // Niet helemaal naar 0, zodat je nog iets ziet
        currentBrightness -= 2; // Dimsnelheid (hoe groter, hoe sneller)
        FastLED.setBrightness(currentBrightness);
        FastLED.show();
    }
}

void BedLightController::restoreWhiteLight() {
    static uint8_t currentBrightness = 0;

    if (currentBrightness < BRIGHTNESS) {
        currentBrightness += 5; // Herstelsnelheid (iets sneller)
        if (currentBrightness > BRIGHTNESS) currentBrightness = BRIGHTNESS;
        FastLED.setBrightness(currentBrightness);

        for (int i = 0; i < _numLeds; i++) {
            _leds[i] = CRGB::White;
        }
        FastLED.show();
    }
}
