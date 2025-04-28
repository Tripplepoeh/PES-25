#include <Arduino.h>
#include "LEDstrip.h"

#define LED_PIN D0
#define SENSOR_PIN A0
#define NUM_LEDS 9

BedLightController bedLightController(LED_PIN, NUM_LEDS, SENSOR_PIN);

void setup() {
  bedLightController.begin();
}

void loop() {
  bedLightController.update();
}
