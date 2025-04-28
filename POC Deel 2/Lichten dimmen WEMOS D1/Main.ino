#include <Arduino.h>       // Arduino-bibliotheek voor algemene functies
#include "LEDstrip.h"      // Headerbestand voor het beheren van de LED-strip

// Definitie van constante parameters
#define LED_PIN D0         // Pin waarop de LED-strip is aangesloten
#define SENSOR_PIN A0      // Pin waarop de druksensor is aangesloten
#define NUM_LEDS 9         // Aantal LEDs in de LED-strip

// Maak een instance van de BedLightController-klasse
// Hiermee wordt de LED-strip en druksensor beheerd.
BedLightController bedLightController(LED_PIN, NUM_LEDS, SENSOR_PIN);

void setup() {
  // Initialiseer de LED-strip en druksensor.
  bedLightController.begin();
}

void loop() {
  // Voer voortdurend de logica uit om de LED-strip en druksensorstatus te beheren.
  bedLightController.update();
}
