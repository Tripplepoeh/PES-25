/*
 * lamp.cpp
 *
 *  Created on: May 11, 2025
 *      Author: aashi
 */
#include "geleLed.h"

geleLed::geleLed(){

}
void geleLed::zetAan(){
	HAL_GPIO_WritePin(GPIOA, geelLed_Pin, GPIO_PIN_SET);
}
void geleLed::zetUit(){
	 HAL_GPIO_WritePin(GPIOA, geelLed_Pin, GPIO_PIN_RESET);

}


