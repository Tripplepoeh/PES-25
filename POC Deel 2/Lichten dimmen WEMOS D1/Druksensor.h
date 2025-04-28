#include "Druksensor.h"

// Constructor van de Druksensor-klasse
// Initialiseert de druksensor en stelt de pin in als invoer.
Druksensor::Druksensor(uint8_t pin) : _pin(pin) {
    pinMode(_pin, INPUT); // Zet de pin als input om analoge waarden te lezen.
}

// Controleer of de druksensor is ingedrukt.
// Retourneert `true` als de analoge waarde groter of gelijk is aan de drempelwaarde (30).
bool Druksensor::isPressed() {
    return analogRead(_pin) >= 30; // Analoge lezing wordt vergeleken met drempelwaarde.
}

// Lees de huidige analoge waarde van de druksensor.
// Retourneert de onbewerkte analoge waarde die door de sensor wordt gelezen.
int Druksensor::getValue() {
    return analogRead(_pin); // Retourneer de actuele waarde van de pin.
}
