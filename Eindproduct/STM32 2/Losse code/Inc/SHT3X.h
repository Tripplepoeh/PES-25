/*
 * SHT3X.h
 *
 *  Created on: May 19, 2025
 *      Author: @tripplepoeh
 *
 */

#ifndef SHT3X_H
#define SHT3X_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx_hal.h"

#define SHT3X_ADDRESS (0x44 << 1) // Shifted voor STM32 HAL

HAL_StatusTypeDef SHT3X_Read(I2C_HandleTypeDef *hi2c, float *temperature, float *humidity);
float getTempWaarde();
float getHumidityWaarde();


#ifdef __cplusplus
}
#endif

#endif // SHT3X_H

