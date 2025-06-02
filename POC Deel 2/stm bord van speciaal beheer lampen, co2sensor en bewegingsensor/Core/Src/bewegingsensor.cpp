/*
 * Bewegingssensor.cpp
 *
 *  Created on: May 12, 2025
 *      Author: ocker
 */

#include "bewegingsensor.h"
#include "string.h"
#include "main.h"

Bewegingssensor::Bewegingssensor(GPIO_TypeDef* gpioPort, uint16_t gpioPin)
    : _gpioPort(gpioPort), _gpioPin(gpioPin), _waarde(0) {}


void Bewegingssensor::CheckWaarde() {
    int status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
    _waarde = status;
}


uint8_t Bewegingssensor::GetWaarde() const {
    return _waarde;
}
