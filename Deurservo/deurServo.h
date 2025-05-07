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

class RFIDSensor; // verbeter naam als nodig
class deurknop;

class deurServo {
private:
	int beweging;
	TIM_HandleTypeDef* pwmkanaal;
	uint32_t tickaantal;
	RFIDSensor* sensor;
	deurknop* knop;

public:
	deurServo(TIM_HandleTypeDef* timer);
	virtual ~deurServo();
	void draaiServo(int begin, int eind, int tijd);
	void setPWM(int pulsebreedte);
	void getRFID(RFIDSensor* sens);
	void getKnop(deurknop* knop);
	void checkOpen();
	uint32_t pwmNaarHoek(int hoek);

};

#endif /* SRC_DEURSERVO_H_ */
