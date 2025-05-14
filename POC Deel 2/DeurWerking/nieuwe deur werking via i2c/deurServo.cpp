#include "deurServotest.h"


deurServotest::deurServotest(TIM_HandleTypeDef* timer)
    : pwmkanaal(timer),
      state(SERVO_IDLE),
      beweging(false),
      beginHoek(0),
      eindHoek(0),
      huidigeStap(0),
      laatsteUpdateTijd(0),
      openTimeout(0),
      staprust(0)
{}


deurServotest::~deurServotest() {}

void deurServotest::setPWM(int pulsbreedte) {
    __HAL_TIM_SET_COMPARE(pwmkanaal, TIM_CHANNEL_1, pulsbreedte);
}

uint32_t deurServotest::pwmNaarHoek(int hoek) {
    return 500 + (hoek * 2000 / 180);
}

void deurServotest::startServoMove(int begin, int eind, uint32_t tijdMs) {
    if (beweging || begin == eind) return;

    beginHoek = begin;
    eindHoek = eind;
    huidigeStap = 0;
    staprust = tijdMs / STAPPEN;
    laatsteUpdateTijd = HAL_GetTick();
    beweging = true;

    setPWM(pwmNaarHoek(beginHoek));
    state = (eind > begin) ? SERVO_OPENING : SERVO_CLOSING;
}

void deurServotest::update() {
    uint32_t huidigeTijd = HAL_GetTick();


    if (!beweging) {
        if (state == SERVO_HOLDING_OPEN && huidigeTijd >= openTimeout ) {
            zetUit(); 
        }
        return;
    }

    if (huidigeTijd - laatsteUpdateTijd >= staprust) {
        if (huidigeStap < STAPPEN) {
            int hoek = beginHoek + ((eindHoek - beginHoek) * huidigeStap) / STAPPEN;
            setPWM(pwmNaarHoek(hoek));
            huidigeStap++;
            laatsteUpdateTijd = huidigeTijd;
        } else {
            setPWM(pwmNaarHoek(eindHoek));
            beweging = false;

            if (eindHoek == 180) {
                state = SERVO_HOLDING_OPEN;
                openTimeout = huidigeTijd + 3000;
            } else {
                state = SERVO_IDLE;
            }
        }
    }
}




void deurServotest::zetAan() {
    if (state == SERVO_IDLE) {
        startServoMove(0, 180, 3000);
    }
}

void deurServotest::zetUit() {
    if (state == SERVO_HOLDING_OPEN) {
        startServoMove(180, 0, 3000);
    }
}

int deurServotest::getWaarde() {
    return (state == SERVO_HOLDING_OPEN) ? 1 : 0;
}
