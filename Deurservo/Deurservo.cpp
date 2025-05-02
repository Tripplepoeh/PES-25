/*
 * deurServo.cpp
 *
 *  Created on: Apr 25, 2025
 *      Author: basti
 */

#include "deurServo.h"


deurServo::deurServo(TIM_HandleTypeDef* timer) { // constructor pakt de timer en begint in non-bewegende modus
	pwmkanaal = timer;
	beweging = false;


}

deurServo::~deurServo() {
	// TODO Auto-generated destructor stub
}

void deurServo::setPWM(int pulsbreedte){ // set de CCR 
	__HAL_TIM_SET_COMPARE(pwmkanaal, TIM_CHANNEL_1, pulse);
}
uint32_t deurServo::pwmNaarHoek(int hoek) { // convert de pwmwaarde naar een hoek van 0-180 die de servo moet aannemen
	return 1000 + (hoek * 1000 / 180);
}

void deurServo::draaiServo(int begin, int eind, int tijd){ // draai de deur van beginhoek tot eindhoek in de gegeven tijd
	if (begin == eind) return; // als beginhoek en eindhoek hetzelfde zijn, return en beweeg niet
	if (!beweging) { // check of de servo al aan het bewegen is. als dit niet zo is dan kan het beginnen met bewegen.
		beweging = true; 
	int staprust = tijd / STAPPEN; // delay tussen stappen
	if (begin > eind) {
		for (int i = 0; i < STAPPEN; i++) {
		int hoekhuidig = begin - ((begin - eind) * i) / STAPPEN; // bereken de huidige hoek van de servo
		uint32_t pwmwaarde = pwmNaarHoek(hoekhuidig); //
		setPWM(pwmwaarde);
		HAL_Delay(staprust);


		}
	}
	else if ( eind > begin) {
		for (int i = 0; i < STAPPEN; i++) {
				int hoekhuidig = begin + ((eind - begin) * i) / STAPPEN;
				uint32_t pwmwaarde = pwmNaarHoek(hoekhuidig);
				setPWM(pwmwaarde);
				HAL_Delay(staprust);
		}
	}
	}
	beweging = false; // geef aan dat de servo is gestopt met bewegen
}
