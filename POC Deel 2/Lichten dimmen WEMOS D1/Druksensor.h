#ifndef DRUKSENSOR_H
#define DRUKSENSOR_H

 #include <Arduino.h>

class Druksensor {
public:
    // Constructor
    Druksensor(uint8_t pin);
    
    // Methode om te controleren of de knop ingedrukt is
    bool isPressed();
    
    // Methode om de waarde van de druksensor te krijgen
    int getValue();
    

private:
    uint8_t _pin;  // De pin waaraan de sensor is aangesloten
};

#endif
