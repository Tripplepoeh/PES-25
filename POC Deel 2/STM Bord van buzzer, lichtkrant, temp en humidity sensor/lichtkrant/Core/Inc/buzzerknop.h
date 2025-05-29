// buzzerknop.h
#ifndef BUZZERKNOP_H
#define BUZZERKNOP_H

#include "main.h"

class Buzzerknop {
public:
    Buzzerknop();
    ~Buzzerknop();

    void poll();         // Debounce & statusupdate
    bool isIngedrukt();  // Retourneert true als knop net is ingedrukt
    void gedrukt();
    void onGedrukt();

private:
    uint32_t lastPollTime;
    uint8_t rawStatus;
    uint8_t stableStatus;
    uint8_t previousStableStatus;
    uint8_t debounceDelay;
    bool knopStatus;
};

#endif
