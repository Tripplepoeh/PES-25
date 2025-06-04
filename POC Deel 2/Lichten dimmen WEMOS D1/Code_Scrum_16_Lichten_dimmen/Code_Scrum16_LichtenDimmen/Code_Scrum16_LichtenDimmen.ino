#include "LEDstrip.h"
#include <FastLED.h>

// Pin waar de LEDstrip is aangesloten
#define LED_PIN D0
// Analoge pin voor de druksensor
#define Druk_Sensor A0
// Aantal LEDs in de strip
#define NUM_LEDS 8
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
  // Voeg LEDs toe aan de FastLED bibliotheek, voor dynamische pin ondersteuning
  FastLED.addLeds<WS2811, D0, GRB>(_leds, _numLeds).setCorrection(TypicalLEDStrip);
  // Stel de helderheid in
  FastLED.setBrightness(64);
}

// Stel de kleur van de LEDstrip in op basis van een situatie
void LEDstrip::setColor(int situation) {
  // Doe niets als de situatie niet veranderd is
  if (_currentState == situation) return;

  // Schakel de LEDstrip aan als die eerst uit stond en nu aan moet
  if (_currentState == -1 && situation == 1) {
    lichtAan();
    _currentState = situation;
    return;
  }
  _currentState = situation;

  // Verander de kleur geleidelijk in stappen
  for (int colorStep = 0; colorStep <= 255; colorStep++) {
    r = 255;
    // Als situatie 1: kleur opbouwen, anders kleur afbouwen
    g = (situation == 1) ? colorStep : 255 - colorStep;
    b = (situation == 1) ? colorStep : 255 - colorStep;

    // Pas de kleur toe op alle LEDs
    for (int i = 0; i < _numLeds; i++) {
      _leds[i] = CRGB(r, g, b);
    }

    FastLED.show();
    delay(1000); // Wacht 1 seconde tussen elke stap voor vloeiende overgang
  }
}

// Update de LEDstrip (laat huidige kleuren zien)
void LEDstrip::update() {
  FastLED.show();
}

// Zet de LEDstrip uit (dim tot lage waarde)
void LEDstrip::lichtUit() {
  for (int i = 0; i < _numLeds; i++) {
    _leds[i] = CRGB(40, 40, 40); // Zwakke gloed (niet volledig uit)
  }
  FastLED.show();
}

// Dim de LEDstrip (zet alle LEDs op rood)
void LEDstrip::LichtDimmen() {
  for (int i = 0; i < _numLeds; i++) {
    _leds[i] = CRGB::Red;
  }
  FastLED.show();
}

// Zet de LEDstrip volledig aan (helder wit)
void LEDstrip::lichtAan() {
  for (int i = 0; i < _numLeds; i++) {
    _leds[i] = CRGB::White;
  }
  FastLED.show();
}
