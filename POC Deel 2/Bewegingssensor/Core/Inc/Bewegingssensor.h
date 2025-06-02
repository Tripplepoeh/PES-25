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
	Bewegingssensor(GPIO_TypeDef* gpiopPort, uint16_t gpioPin, I2C_HandleTypeDef* i2cHandle);

//	void controleerEnVerzend();
	void CheckWaarde();
	uint8_t GetWaarde() const;

private:
	GPIO_TypeDef* _gpioPort;
	uint16_t _gpioPin;
	I2C_HandleTypeDef* _i2c;
	uint8_t _waarde;

};



#endif /* INC_BEWEGINGSSENSOR_H_ */
