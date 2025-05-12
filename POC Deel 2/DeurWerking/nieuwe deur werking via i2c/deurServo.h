


/*
 * deurServo.h
 *
 *  Created on: Apr 25, 2025
 *      Author: basti
 */

#ifndef SRC_DEURSERVO_H_
#define SRC_DEURSERVO_H_
#include <stdio.h>
#include <stdint.h>
extern "C" {
#include "stm32l4xx_hal.h"
}
#define STAPPEN 100

class RFIDSensor;
class deurknop;

//class deurServo {
//private:
//int beweging;
//int status;
//TIM_HandleTypeDef* pwmkanaal;
//uint32_t tickaantal;
//RFIDSensor* sensor;
//deurknop* knop;
//
//
//public:
//deurServo(TIM_HandleTypeDef* timer);
//virtual ~deurServo();
//void draaiServo(int begin, int eind, int tijd);
//void setPWM(int pulsebreedte);
//void getRFID(RFIDSensor* sens);
//void getKnop(deurknop* knop);
//void checkOpen();
//uint32_t pwmNaarHoek(int hoek);
//void zetAan();
//void zetUit();
//int getWaarde();
//
//};
//
enum ServoState {
    SERVO_IDLE,
    SERVO_OPENING,
    SERVO_OPEN,
    SERVO_CLOSING
};
class deurServo {
public:
    deurServo(TIM_HandleTypeDef* timer);
    ~deurServo();

    void setPWM(int pulsbreedte);
    uint32_t pwmNaarHoek(int hoek);
    void draaiServo(int begin, int eind, int tijd);
    void zetAan();
    void zetUit();
    int getWaarde();
    void update();  // New function to handle non-blocking update

private:
    TIM_HandleTypeDef* pwmkanaal;
    ServoState state;  // State of the servo
    uint32_t targetPwm;  // Target PWM value
    int currentAngle;  // Current angle of the servo
    uint32_t lastUpdateTime;  // Last update time for non-blocking delay
    bool beweging;  // To track if movement is in progress
    uint32_t openTime;  // Time to keep the door open
    int targetAngle;         // Doelhoek waarnaar de servo moet bewegen
    int stapTijd;
};
#endif /* SRC_DEURSERVO_H_ */


