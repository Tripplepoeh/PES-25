/*
 * Bewegingssensor.cpp
 *
 *  Created on: May 12, 2025
 *      Author: ocker
 */

#include "Bewegingssensor.h"
#include "string.h"

Bewegingssensor::Bewegingssensor(GPIO_TypeDef* gpioPort, uint16_t gpioPin, I2C_HandleTypeDef* i2cHandle)
    : _gpioPort(gpioPort), _gpioPin(gpioPin), _i2c(i2cHandle) {}


void Bewegingssensor::CheckWaarde() {
    int status = HAL_GPIO_ReadPin(_gpioPort, _gpioPin);
    _waarde = (status == GPIO_PIN_SET) ? 0x01 : 0x00;
}


uint8_t Bewegingssensor::GetWaarde() const {
    return _waarde;
}
