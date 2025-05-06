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
    delay(10);
  }
}

// Update de LEDstrip (laat de huidige kleur zien)
void LEDstrip::update() {
  FastLED.show();
}

// Zet de LEDstrip uit met een fade-effect
void LEDstrip::lichtUit() {
  // Bepaal de huidige helderheid van 1 LED als startpunt
  int startR = _leds[0].r;
  int startG = _leds[0].g;
  int startB = _leds[0].b;

  // Verlaag geleidelijk de helderheid naar 0
  for (int fade = 0; fade <= startR || fade <= startG || fade <= startB; fade++) {
    int r = (startR - fade) < 0 ? 0 : startR - fade;
    int g = (startG - fade) < 0 ? 0 : startG - fade;
    int b = (startB - fade) < 0 ? 0 : startB - fade;

    for (int i = 0; i < _numLeds; i++) {
      _leds[i] = CRGB(r, g, b);
    }

    FastLED.show();
    delay(10);
  }

  _currentState = -1; // Zet de huidige staat naar 'uitgeschakeld'
}

// Zet de LEDstrip aan met een fade-effect
void LEDstrip::lichtAan() {
  // Bepaal de huidige helderheid van 1 LED als startpunt
  int startR = _leds[0].r;
  int startG = _leds[0].g;
  int startB = _leds[0].b;

  // Verhoog geleidelijk de helderheid naar maximaal
  for (int fade = 0; fade < 255; fade++) {
    int r = startR + fade;
    int g = startG + fade;
    int b = startB + fade;

    for (int i = 0; i < _numLeds; i++) {
      _leds[i] = CRGB(r, g, b);
    }

    FastLED.show();
    delay(10);
  }

  _currentState = -1; // Zet de huidige staat naar 'ingeschakeld'
}
