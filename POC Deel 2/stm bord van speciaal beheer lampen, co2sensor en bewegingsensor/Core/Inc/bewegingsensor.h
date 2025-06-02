/*
 * bewegingsensor.h
 *
 *  Created on: Jun 2, 2025
 *      Author: aashi
 */

#ifndef INC_BEWEGINGSENSOR_H_
#define INC_BEWEGINGSENSOR_H_

#include "main.h"

class Bewegingssensor {
public:
	Bewegingssensor(GPIO_TypeDef* gpiopPort, uint16_t gpioPin);

//	void controleerEnVerzend();
	void CheckWaarde();
	uint8_t GetWaarde() const;

private:
	GPIO_TypeDef* _gpioPort;
	uint16_t _gpioPin;
	//I2C_HandleTypeDef* _i2c;
	uint8_t _waarde;

};




#endif /* INC_BEWEGINGSENSOR_H_ */
