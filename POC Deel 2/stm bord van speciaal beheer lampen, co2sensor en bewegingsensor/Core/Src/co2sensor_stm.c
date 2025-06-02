/*
 * co2sensor_stm.c
 *
 *  Created on: May 29, 2025
 *      Author: aashi
 */
#include "co2sensor_stm.h"
#include "crc8.h"
#include "main.h"

static uint16_t co2Waarde = 0;
static uint16_t tvocWaarde = 0;

extern I2C_HandleTypeDef hi2c3;

void SGP30_init(void) {
    HAL_Delay(50);
    uint8_t cmd[2] = {0x20, 0x03};
    HAL_I2C_Master_Transmit(&hi2c3, SGP30_ADDRESS, cmd, 2, HAL_MAX_DELAY);
    HAL_Delay(10);
}

bool SGP30_read(uint16_t *co2, uint16_t *tvoc) {
    uint8_t cmd[2] = {0x20, 0x08};
    if (HAL_I2C_Master_Transmit(&hi2c3, SGP30_ADDRESS, cmd, 2, HAL_MAX_DELAY) != HAL_OK)
        return false;
    HAL_Delay(12);
    uint8_t data[6];
    if (HAL_I2C_Master_Receive(&hi2c3, SGP30_ADDRESS, data, 6, HAL_MAX_DELAY) != HAL_OK)
        return false;

    if (sensirion_crc8(data, 2) != data[2] ||
        sensirion_crc8(data + 3, 2) != data[5])
        return false;

    *co2 = co2Waarde = (data[0] << 8) | data[1];
    *tvoc = tvocWaarde = (data[3] << 8) | data[4];
    return true;
}

uint16_t getCo2Waarde(){
	return co2Waarde;

}

