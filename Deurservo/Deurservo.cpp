/*
 * deurServo.cpp
 *
 *  Created on: Apr 25, 2025
 *      Author: basti
 */

#include <RFIDSensor.hpp> // verbeter naam als nodig
#include "deurServo.h"
#include "deurknop.h"

deurServo::deurServo(TIM_HandleTypeDef* timer) {
	pwmkanaal = timer;
	beweging = false;


}

deurServo::~deurServo() {
	// TODO Auto-generated destructor stub
}
//deurServo::getRFID(RFIDSensor* sens) {
//	sensor = sens;
//}

void deurServo::getKnop(deurknop* k) {
	knop = k;
}
void deurServo::checkOpen(){
	if (!(knop->statusopen())) {
		draaiServo(0, 180, 3000);
		draaiServo(180, 0, 3000);

	}
//	else if (sensor->CheckKaart())  {
//		if (sensor->CheckToegang()){
//		draaiServo(0, 180, 3000);
//		draaiServo(180, 0, 3000);
//		}
//	}
}

void deurServo::setPWM(int pulsbreedte){
	tickaantal = pulsbreedte;
	__HAL_TIM_SET_COMPARE(pwmkanaal, TIM_CHANNEL_1, tickaantal);
}
uint32_t deurServo::pwmNaarHoek(int hoek) {
	return 1000 + (hoek * 1000 / 180);
}

void deurServo::draaiServo(int begin, int eind, int tijd){
	if (begin == eind) return;
	if (!beweging) {
		beweging = true;
	int staprust = tijd / STAPPEN;
		for (int i = 0; i < STAPPEN; i++) {
				int hoekhuidig = begin + ((eind - begin) * i) / STAPPEN;
				uint32_t pwmwaarde = pwmNaarHoek(hoekhuidig);
				setPWM(pwmwaarde);
				HAL_Delay(staprust);
		}
		uint32_t pwmwaarde = pwmNaarHoek(eind);
		setPWM(pwmwaarde);

	}

	beweging = false;
	HAL_Delay(3000);
}
