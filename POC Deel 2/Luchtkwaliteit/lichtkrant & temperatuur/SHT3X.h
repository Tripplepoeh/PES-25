#ifndef SHT3X_H
#define SHT3X_H

#include <stdint.h>

#define SHT3X_ADDRESS 0x44 // Default I2C address for SHT3X
// #define SHT3X_ADDRESS (0x44 << 1)

HAL_StatusTypeDef SHT3X_Read(I2C_HandleTypeDef *hi2c, float *temperature, float *humidity);

#endif // SHT3X_H