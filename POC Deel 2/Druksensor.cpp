#include "Druksensor.h"

Druksensor::Druksensor(uint8_t sensorPin)
    : _sensorPin(sensorPin) {
}

void Druksensor::begin() {
    pinMode(_sensorPin, INPUT);
}

bool Druksensor::isPersonDetected() {
    int sensorValue = analogRead(_sensorPin);
    return sensorValue >= 30; // Drempelwaarde om te detecteren
}
