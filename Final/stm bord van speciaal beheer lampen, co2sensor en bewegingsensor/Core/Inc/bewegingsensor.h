/*
 * Bewegingssensor.h
 *
 *  Created on: May 12, 2025
 *      Author: ocker
 */

#ifndef INC_BEWEGINGSSENSOR_H_
#define INC_BEWEGINGSSENSOR_H_

#include "main.h"

class Bewegingssensor {
public:
    Bewegingssensor();

//    void controleerEnVerzend();
    void CheckWaarde();
    uint8_t GetWaarde() const;

private:

    uint8_t _waarde;

};



#endif /* INC_BEWEGINGSSENSOR_H_ */
