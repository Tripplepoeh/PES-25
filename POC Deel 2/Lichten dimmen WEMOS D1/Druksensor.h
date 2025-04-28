#include "Druksensor.h"

// Constructor voor de Druksensor-klasse.
// Initialiseert de drukgevoelige sensor met de opgegeven pin.
Druksensor::Druksensor(uint8_t sensorPin)
    : _sensorPin(sensorPin) {
}

// Initialiseer de sensor.
// Stel de opgegeven pin in als een invoer (INPUT) voor het lezen van analoge waarden.
void Druksensor::begin() {
    pinMode(_sensorPin, INPUT);
}

// Controleer of een persoon is gedetecteerd door de druksensor.
// Leest de analoge waarde van de sensor en vergelijkt deze met een drempelwaarde.
// Als de waarde gelijk aan of hoger dan 30 is, wordt aangenomen dat een persoon is gedetecteerd.
bool Druksensor::isPersonDetected() {
    int sensorValue = analogRead(_sensorPin);
    return sensorValue >= 30; // Drempelwaarde om te detecteren
}
