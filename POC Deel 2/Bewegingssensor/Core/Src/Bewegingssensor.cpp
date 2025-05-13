/*
 * Bewegingssensor.cpp
 *
 *  Created on: May 12, 2025
 *      Author: ocker
 */

#include "Bewegingssensor.h"
#include "string.h"

Bewegingssensor::Bewegingssensor(GPIO_TypeDef* gpioPort, uint16_t gpioPin, UART_HandleTypeDef* uartHandle)
: _gpioPort(gpioPort), _gpioPin(gpioPin), _uart(uartHandle) {}

void Bewegingssensor::controleerEnVerzend() {
int status = HAL_GPIO_ReadPin(_gpioPort, _gpioPin);
const char* msg = (status == 1) ? msgBeweging : msgGeenBeweging;

HAL_UART_Transmit(_uart, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
const char newline[] = "\r\n";
HAL_UART_Transmit(_uart, (uint8_t*)newline, strlen(newline), HAL_MAX_DELAY);
}


