#include "LEDstrip.h"
#include <FastLED.h>

// Pin waar de LEDstrip is aangesloten
#define LED_PIN D0
// Analoge pin voor de druksensor
#define Druk_Sensor A0
// Aantal LEDs in de strip
#define NUM_LEDS 9
// Standaard helderheid van de LEDs
#define BRIGHTNESS 64
// Type LEDstrip
#define LED_TYPE WS2811
// Kleurenvolgorde van de LEDs
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

// Constructor voor de LEDstrip klasse
LEDstrip::LEDstrip(uint8_t pin, uint16_t numLeds)
  : _pin(pin), _numLeds(numLeds), _currentState(0) {
  // Reserveer geheugen voor de LED array
  _leds = new CRGB[_numLeds];
}

// Initialiseer de LEDstrip
void LEDstrip::init() {
  pinMode(_pin, OUTPUT);
  // Voeg LEDs toe aan de FastLED bibliotheek
  // FastLED voegt ondersteuning toe voor dynamische pinnen
  FastLED.addLeds<WS2811, D0, GRB>(_leds, _numLeds).setCorrection(TypicalLEDStrip);
  // Stel de helderheid in
  FastLED.setBrightness(64);
}

// Stel de kleur van de LEDstrip in op basis van een situatie
void LEDstrip::setColor(int situation) {
  // Voorkom het aanpassen als de huidige situatie overeenkomt
  if (_currentState == situation) return;

  // Schakel de LEDstrip aan als de vorige staat uitgeschakeld was
  if (_currentState == -1 && situation == 1) {
    lichtAan();
    _currentState = situation;
    return;
  }
  _currentState = situation;

  // Verander de kleur geleidelijk
  for (int colorStep = 0; colorStep <= 255; colorStep++) {
    r = 255;
    g = (situation == 1) ? colorStep : 255 - colorStep;
    b = (situation == 1) ? colorStep : 255 - colorStep;

    // Pas de kleur toe op alle LEDs
    for (int i = 0; i < _numLeds; i++) {
      _leds[i] = CRGB(r, g, b);
    }

    FastLED.show();
    delay(1000);
  }
}

// Update de LEDstrip (laat de huidige kleur zien)
void LEDstrip::update() {
  FastLED.show();
}

void LEDstrip::lichtUit() {
    for (int i = 0; i < _numLeds; i++) {
        _leds[i] = CRGB::Black;
    }
    FastLED.show();
}

void LEDstrip::LichtDimmen() {
    for (int i = 0; i < _numLeds; i++) {
        _leds[i] = CRGB::Red;
    }
    FastLED.show();
}

void LEDstrip::lichtAan() {
    for (int i = 0; i < _numLeds; i++) {
        _leds[i] = CRGB::White;
    }
    FastLED.show();
}
