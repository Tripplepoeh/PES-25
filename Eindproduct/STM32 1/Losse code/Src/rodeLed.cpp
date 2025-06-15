/*
 * lamp.cpp
 *
 *  Created on: May 11, 2025
 *      Author: aashi
 */
#include "rodeLed.h"

rodeLed::rodeLed(){

}
void rodeLed::zetAan(){
	HAL_GPIO_WritePin(GPIOB, roodLed_Pin, GPIO_PIN_SET);
}
void rodeLed::zetUit(){
	HAL_GPIO_WritePin(GPIOB, roodLed_Pin, GPIO_PIN_RESET);

}


