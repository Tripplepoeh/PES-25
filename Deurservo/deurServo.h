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
	int beweging; // variabele die checkt of de servo al aan het bewegen is.
	TIM_HandleTypeDef* pwmkanaal; // variabele waarin de timer wordt opgeslagen.

public:
	deurServo(TIM_HandleTypeDef* timer);
	virtual ~deurServo();
	void draaiServo(int begin, int eind, int tijd); // draait de servo van beginhoek naar eindhoek in de gegeven tijd in milisec.
	void setPWM(int pulsebreedte); // set de CCR waarde
	uint32_t pwmNaarHoek(int hoek); // berekent de hoek om de CCR waarde te setten
};

#endif /* SRC_DEURSERVO_H_ */
