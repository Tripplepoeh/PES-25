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

class deurServo {
private:
	int beweging;
	TIM_HandleTypeDef* pwmkanaal;
	void setPWM(int pulsebreedte);
	uint32_t pwmNaarHoek(int hoek);

public:
	deurServo(TIM_HandleTypeDef* timer);
	virtual ~deurServo();
	void draaiServo(int begin, int eind, int tijd);

};

#endif /* SRC_DEURSERVO_H_ */
