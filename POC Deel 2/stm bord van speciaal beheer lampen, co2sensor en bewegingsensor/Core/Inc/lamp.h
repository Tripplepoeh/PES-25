/*
 * lamp.h
 *
 *  Created on: May 11, 2025
 *      Author: aashi
 */

#ifndef INC_LAMP_H_
#define INC_LAMP_H_
#include "main.h"

class lamp{
public:
	virtual ~lamp(){}
	virtual void zetAan() = 0;
	virtual void zetUit() = 0;
};




#endif /* INC_LAMP_H_ */
