/*
 * SHT3X.c
 *
 *  Created on: May 19, 2025
 *      Author: @tripplepoeh
 */


#include "../../../Lichtkrant_STM/Core/Inc/SHT3X.h"

#include "main.h"

HAL_StatusTypeDef SHT3X_Read(I2C_HandleTypeDef *hi2c3, float *temperature, float *humidity) {
    uint8_t cmd[2] = {0x2C, 0x06};
    uint8_t data[6];
    HAL_StatusTypeDef status;

    status = HAL_I2C_Master_Transmit(hi2c3, SHT3X_ADDRESS, cmd, sizeof(cmd), HAL_MAX_DELAY);
    if (status != HAL_OK) return status;
    HAL_Delay(15);

    // Lees de Data
    status = HAL_I2C_Master_Receive(hi2c3, SHT3X_ADDRESS, data, sizeof(data), HAL_MAX_DELAY);
    if (status != HAL_OK) return status;

    uint16_t raw_temp = (data[0] << 8) | data[1];
    uint16_t raw_hum = (data[3] << 8) | data[4];
    *temperature = -45 + (175 * raw_temp / 65535.0);
    *humidity = 100 * raw_hum / 65535.0;
    return HAL_OK;
}
