#include "deurServo.h"
#include "main.h"
#include <cstdlib>  // voor abs()


// Constructor
deurServo::deurServo(TIM_HandleTypeDef* timer)
    : pwmkanaal(timer), state(SERVO_IDLE), currentAngle(0),
      targetAngle(0), lastUpdateTime(0), beweging(false),
      stapTijd(0), openTime(0) {}

// Destructor
deurServo::~deurServo() {}

// Set PWM
void deurServo::setPWM(int pulsbreedte) {
    __HAL_TIM_SET_COMPARE(pwmkanaal, TIM_CHANNEL_1, pulsbreedte);
}

// Convert angle to PWM
uint32_t deurServo::pwmNaarHoek(int hoek) {
    return 500 + (hoek * 2000 / 180);
}

// Start beweging
void deurServo::draaiServo(int begin, int eind, int tijd) {
    if (beweging || begin == eind) return;

    currentAngle = begin;
    targetAngle = eind;
    state = (eind > begin) ? SERVO_OPENING : SERVO_CLOSING;
    stapTijd = tijd / abs(eind - begin); // tijd per graad
    lastUpdateTime = HAL_GetTick();
    beweging = true;
}

// Open de deur
void deurServo::zetAan() {
    if (state == SERVO_IDLE) {
        draaiServo(currentAngle, 180, 3000);
    }
}

// Sluit de deur
void deurServo::zetUit() {
    if (state == SERVO_OPEN) {
        draaiServo(currentAngle, 0, 3000);
    }
}

// Waarde opvragen
int deurServo::getWaarde() {
    return (state == SERVO_OPEN) ? 1 : 0;
}

// Update - moet regelmatig aangeroepen worden in main loop
void deurServo::update() {
    uint32_t currentTime = HAL_GetTick();

    switch (state) {
        case SERVO_OPENING:
        case SERVO_CLOSING:
            if ((int)(currentTime - lastUpdateTime) >= stapTijd) {
                lastUpdateTime = currentTime;

                if (currentAngle != targetAngle) {
                    currentAngle += (state == SERVO_OPENING) ? 1 : -1;
                    setPWM(pwmNaarHoek(currentAngle));
                }

                if (currentAngle == targetAngle) {
                    beweging = false;

                    if (state == SERVO_OPENING) {
                        state = SERVO_OPEN;
                        openTime = currentTime + 3000; // 3 seconden open
                    } else if (state == SERVO_CLOSING) {
                        state = SERVO_IDLE;
                    }
                }
            }
            break;

        case SERVO_OPEN:
            if (currentTime >= openTime) {
                zetUit();  // start sluiten
            }
            break;

        case SERVO_IDLE:
        default:
            // niets doen
            break;
    }
}
