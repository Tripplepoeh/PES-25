/*
 * lamp.cpp
 *
 *  Created on: May 11, 2025
 *      Author: aashi
 */
#include "groeneLed.h"

groeneLed::groeneLed(){

}
void groeneLed::zetAan(){
	HAL_GPIO_WritePin(groenLed_GPIO_Port, groenLed_Pin, GPIO_PIN_SET);
}
void groeneLed::zetUit(){
	HAL_GPIO_WritePin(groenLed_GPIO_Port, groenLed_Pin, GPIO_PIN_RESET);

}


