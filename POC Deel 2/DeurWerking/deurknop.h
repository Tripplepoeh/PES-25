/*
 * deurknop.h
 *
 *  Created on: May 7, 2025
 *      Author: basti
 */

#ifndef INC_DEURKNOP_H_
#define INC_DEURKNOP_H_
extern "C" {
#include "stm32l4xx_hal.h"
}


class deurServo;

class deurknop {
public:
	deurknop();
	~deurknop();
	int statusopen();
	//void opendeur(int);

private:
	GPIO_PinState knopstatus;
	deurServo* deur;
	int oudetoestand;

};




#endif /* INC_DEURKNOP_H_ */
