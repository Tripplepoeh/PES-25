#include "grondvochtigheidSensor.h"

grondvochtigheidSensor::grondvochtigheidSensor() {
}

grondvochtigheidSensor::~grondvochtigheidSensor() {
}

void grondvochtigheidSensor::leesSensor() {
  grondWaarde = analogRead(grondPin); //analoge read van de grondvochtigheidsensor 
  Serial.println(grondWaarde);
}

int grondvochtigheidSensor::getWaarde() {
  return grondWaarde;
}
