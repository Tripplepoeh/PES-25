#ifndef DRUKSENSOR_H
#define DRUKSENSOR_H

#include <Arduino.h>

// Definitie van de Druksensor-klasse.
// Deze klasse wordt gebruikt om een druksensor te beheren en detecteren
// of een persoon wordt gedetecteerd met behulp van een analoge waarde.
class Druksensor {
public:
    // Constructor: Initialiseert de druksensor met een specifieke pin.
    // sensorPin: De pin waarop de druksensor is aangesloten.
    Druksensor(uint8_t sensorPin);

    // Initialiseer de druksensor.
    // Stelt de pin in als invoer (INPUT) zodat analoge waarden kunnen worden gelezen.
    void begin();

    // Controleer of een persoon wordt gedetecteerd.
    // @return true als de sensor een waarde boven de drempel detecteert, anders false.
    bool isPersonDetected();

private:
    uint8_t _sensorPin; // Pin waarop de druksensor is aangesloten.
};

#endif // DRUKSENSOR_H
