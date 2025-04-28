#include "Druksensor.h"

Druksensor::Druksensor(uint8_t pin) : _pin(pin) {
    pinMode(_pin, INPUT);
}

bool Druksensor::isPressed() {
    return analogRead(_pin) >= 30; // Drempelwaarde voor druk
}

int Druksensor::getValue() {
    return analogRead(_pin);
}
