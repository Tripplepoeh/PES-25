#include "LEDstrip.h"
#include <FastLED.h>

#define LED_PIN D0
#define Druk_Sensor A0
#define NUM_LEDS 9
#define BRIGHTNESS 64
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

LEDstrip::LEDstrip(uint8_t pin, uint16_t numLeds)
  : _pin(pin), _numLeds(numLeds), _currentState(0) {
  _leds = new CRGB[_numLeds];
}

void LEDstrip::init() {
  pinMode(_pin, OUTPUT);
  // Call addLeds with a runtime pin argument instead of a template argument
  // FastLED.addLeds<WS2811, _pin, GRB>(_leds, _numLeds).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2811, D0, GRB>(_leds, _numLeds).setCorrection(TypicalLEDStrip);
  ;  // Replace `6` with the actual pin number for hardware SPI if needed
  //FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(64);
}

void LEDstrip::setColor(int situation) {
  if (_currentState == situation) return;


  if (_currentState == -1 && situation == 1) {
    lichtAan();
    _currentState = situation;
    return;
  }
  _currentState = situation;

  for (int colorStep = 0; colorStep <= 255; colorStep++) {
    r = 255;
    g = (situation == 1) ? colorStep : 255 - colorStep;
    b = (situation == 1) ? colorStep : 255 - colorStep;

    for (int i = 0; i < _numLeds; i++) {
      _leds[i] = CRGB(r, g, b);
    }

    FastLED.show();
    delay(10);
  }
}

void LEDstrip::update() {
  FastLED.show();
}

void LEDstrip::lichtUit() {
  // Bepaal de huidige helderheid van 1 LED als startpunt
  int startR = _leds[0].r;
  int startG = _leds[0].g;
  int startB = _leds[0].b;

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

  _currentState = -1;
}

void LEDstrip::lichtAan() {
  // Bepaal de huidige helderheid van 1 LED als startpunt
  int startR = _leds[0].r;
  int startG = _leds[0].g;
  int startB = _leds[0].b;

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

  _currentState = -1;
}
