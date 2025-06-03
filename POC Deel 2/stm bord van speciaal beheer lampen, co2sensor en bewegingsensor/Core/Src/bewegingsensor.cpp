/*
 * Bewegingssensor.cpp
 *
 *  Created on: May 12, 2025
 *      Author: ocker
 */

#include "bewegingsensor.h"
#include "string.h"
#include "main.h"

Bewegingssensor::Bewegingssensor() {}


void Bewegingssensor::CheckWaarde() {
    _waarde = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);

}


uint8_t Bewegingssensor::GetWaarde() const {
    return _waarde;
}
