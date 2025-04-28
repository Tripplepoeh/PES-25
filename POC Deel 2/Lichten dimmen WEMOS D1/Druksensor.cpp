#include "Druksensor.h"

// Constructor van de Druksensor-klasse
// Initialiseert de druksensor en stelt de bijbehorende pin in als invoer.
Druksensor::Druksensor(uint8_t pin) : _pin(pin) {
    pinMode(_pin, INPUT); // Configureer de opgegeven pin als invoer voor analoge metingen.
}

// Controleer of de druksensor is ingedrukt.
// Retourneert `true` als de gemeten analoge waarde groter of gelijk is aan de drempelwaarde (30).
bool Druksensor::isPressed() {
    return analogRead(_pin) >= 30; // Vergelijk de analoge waarde met de drempel van 30.
}

// Lees de huidige analoge waarde van de druksensor.
// Retourneert de onbewerkte analoge waarde die door de sensor wordt gemeten.
int Druksensor::getValue() {
    return analogRead(_pin); // Lees en retourneer de actuele waarde van de pin.
}
