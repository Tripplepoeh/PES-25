// buzzer.h
#ifndef BUZZER_H
#define BUZZER_H

#include "main.h"

class Buzzer {
public:
    Buzzer(TIM_HandleTypeDef* timer, uint32_t channel, uint32_t freqHz);
    void zetAan();
    void zetUit();
    bool isActief() const;

    void knopAan();
    void noodknopAan();
    void knopUit();
    void noodknopUit();


private:
    TIM_HandleTypeDef* _timer;
    uint32_t _channel;
    uint32_t _arr;
    bool _actief;
    bool knopStatus;
    bool noodknopStatus;
};

#endif
