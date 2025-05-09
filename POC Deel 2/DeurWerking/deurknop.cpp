#include "deurknop.h"
#include "deurServo.h"

deurknop::deurknop(){

}

deurknop::~deurknop(){

}

int deurknop::statusopen(){
	knopstatus = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
	return knopstatus;

}

//void deurknop::opendeur(int knop){
//	knopstatus = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
//	if (knopstatus) {
//			  deur ->draaiServo(0, 180, 3000);
//			  HAL_Delay(3000);
//			  deur ->draaiServo(180, 0, 3000);
//			  HAL_Delay(3000);
//	}
//}

