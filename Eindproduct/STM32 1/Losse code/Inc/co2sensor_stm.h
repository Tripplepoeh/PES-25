/*
 * co2sensor_stm.h
 *
 *  Created on: May 29, 2025
 *      Author: aashi
 */

#ifndef INC_CO2SENSOR_STM_H_
#define INC_CO2SENSOR_STM_H_

#include "crc8.h"
#include <stdbool.h>

#define SGP30_ADDRESS (0x58 << 1) // SGP30 I2C address shifted left by 1

#ifdef __cplusplus
extern "C" {
#endif

void SGP30_init(void);
bool SGP30_read(uint16_t *co2, uint16_t *tvoc);
uint16_t getCo2Waarde(void);

#ifdef __cplusplus
}
#endif
//uint16 getCo2Waarde();



#endif /* INC_CO2SENSOR_STM_H_ */
