/*
 * deurServotest.h
 *
 *  Created on: May 13, 2025
 *      Author: basti
 */

#ifndef DEURSERVOTEST_H
#define DEURSERVOTEST_H

#include "stm32l4xx_hal.h"
#include "main.h"
#include "RFIDSensor.hpp"

#define STAPPEN 100

enum ServoState {
    SERVO_IDLE,
    SERVO_OPENING,
    SERVO_HOLDING_OPEN,
    SERVO_CLOSING
};

class deurknoptest;
class RFIDSensor;

class deurServotest {
public:
    deurServotest(TIM_HandleTypeDef* timer);
    ~deurServotest();

    void update();           // Call this repeatedly
    void checkOpen();        // Checks button and RFID
    void zetAan();
    void zetUit();
    int getWaarde();         // 1 = open, 0 = closed
    void setPWM(int pulsbreedte);
    uint32_t pwmNaarHoek(int hoek);
    void startServoMove(int begin, int eind, uint32_t tijdMs);


private:
    TIM_HandleTypeDef* pwmkanaal;
    ServoState state;
    bool beweging;

    int beginHoek, eindHoek, huidigeStap;
    uint32_t laatsteUpdateTijd, openTimeout, staprust;

};

#endif
