#ifndef INC_DEURKNOP_H_
#define INC_DEURKNOP_H_

extern "C" {
#include "stm32l4xx_hal.h"
}

#include <cstdint>

class deurknop {
public:
    deurknop();          // Constructor
    ~deurknop();         // Destructor

    void poll();         // Functie om de knopstatus te lezen (debounce)
    uint8_t isOpen() const;  // Functie om de huidige stabiele status van de knop te krijgen (0 = gesloten, 1 = open)

private:
    uint32_t lastPollTime;   // Tijd van de laatste statusverandering
    uint8_t rawStatus;       // Huidige ongefilterde status van de knop (0 of 1)
    uint8_t stableStatus;    // De stabiele status na debouncing
    const uint32_t debounceDelay; // Debounce vertraging in milliseconden
    GPIO_PinState knopstatus;  // De status van de knop in termen van GPIO_PinState (0 of 1)

    // Voeg eventueel extra functies toe voor debouncing of configuratie
};

#endif /* INC_DEURKNOP_H_ */
