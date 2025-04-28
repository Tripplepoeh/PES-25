#ifndef DRUKSENSOR_H
#define DRUKSENSOR_H

#include <Arduino.h> // Arduino-bibliotheek voor hardwarefunctionaliteit

// Definitie van de Druksensor-klasse
// Deze klasse vertegenwoordigt een druksensor die kan detecteren of er druk is uitgeoefend,
// en ook de exacte waarde van de druk kan uitlezen.
class Druksensor {
public:
    // Constructor
    // Initialiseert de druksensor met de opgegeven pin waarop deze is aangesloten.
    // @param pin: De pin waaraan de druksensor is gekoppeld.
    Druksensor(uint8_t pin);
    
    // Methode om te controleren of de knop/sensor is ingedrukt.
    // Retourneert true als de sensor een druk detecteert die boven een bepaalde drempelwaarde ligt.
    bool isPressed();
    
    // Methode om de analoge waarde van de druksensor te verkrijgen.
    // Retourneert de onbewerkte waarde die door de sensor wordt gelezen.
    int getValue();

private:
    uint8_t _pin;  // De pin waaraan de druksensor is gekoppeld.
};

#endif // Beëindiging van de header guard
