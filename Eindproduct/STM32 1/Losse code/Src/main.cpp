#include "i2cBeheer.hpp"
#include "groeneLed.h"
#include "geleLed.h"
#include "rodeLed.h"
#include <vector>
#include "alleDefines.h"
#include "bewegingsensor.h"

extern "C" {
    #include "crc8.h"
    #include "co2sensor_stm.h"
}
#include <stdio.h>
#include <string.h>

I2CBeheer i2c;

int main(void)
{
    uint8_t scrollData[200];   // Buffer voor scrollkolommen
    int len_str, len_num, total_len;

    for (uint8_t addr = 1; addr < 0x78; addr++) {
        if (HAL_I2C_IsDeviceReady(&hi2c3, addr << 1, 3, 100) == HAL_OK) {
            char s[32];
            int n = snprintf(s, sizeof(s), "Found device at 0x%02X\r\n", addr);
            HAL_UART_Transmit(&huart2, (uint8_t*)s, n, HAL_MAX_DELAY);
        }
    }
    uint16_t co2, tvoc;
    char init_msg[] = "SGP30 gestart\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)init_msg, strlen(init_msg), HAL_MAX_DELAY);
    char msg[64];

    SGP30_init();

    HAL_GPIO_WritePin(groenLed_GPIO_Port, groenLed_Pin, GPIO_PIN_SET);

    groeneLed groen;
    geleLed geel;
    rodeLed rood;

    Bewegingssensor bws;

    std::vector<uint8_t> Ids = { BEWEGINGSENSOR, CO2SENSOR, ROODLAMP, GROENLAMP, GEELLAMP };
    i2c.I2CInit(Ids, &geel, &groen, &rood, &bws);

    HAL_I2C_EnableListen_IT(&hi2c1);

    while (1)
    {
        bws.CheckWaarde();
        if (SGP30_read(&co2, &tvoc) == 1) {
            int len = snprintf(msg, sizeof(msg), "eCO2: %u ppm, TVOC: %u ppb\r\n", co2, tvoc);
            HAL_UART_Transmit(&huart2, (uint8_t*)msg, len, HAL_MAX_DELAY);
        } else {
            char err_msg[] = "SGP30 leesfout\r\n";
            HAL_UART_Transmit(&huart2, (uint8_t*)err_msg, strlen(err_msg), HAL_MAX_DELAY);
        }
    }
}
