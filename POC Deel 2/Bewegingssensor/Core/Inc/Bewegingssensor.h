/*
 * Bewegingssensor.h
 *
 *  Created on: May 12, 2025
 *      Author: ocker
 */

#ifndef INC_BEWEGINGSSENSOR_H_
#define INC_BEWEGINGSSENSOR_H_

#include "main.h"

class Bewegingssensor {
public:
	Bewegingssensor(GPIO_TypeDef* gpiopPort, uint16_t gpioPin, UART_HandleTypeDef* uartHandle);
	void controleerEnVerzend();

private:
	GPIO_TypeDef* _gpioPort;
	uint16_t _gpioPin;
	UART_HandleTypeDef* _uart;
	const char* msgBeweging = "Beweging gedetecteerd.";
	const char* msgGeenBeweging = "Geen beweging gedetecteerd.";
};



#endif /* INC_BEWEGINGSSENSOR_H_ */
