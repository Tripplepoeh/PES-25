#include "crc8.h"
#include <stdbool.h>

#define SGP30_ADDRESS (0x58 << 1) // SGP30 I2C address shifted left by 1
char i2c_line = &hi2c1; 


void SGP30_init(void) {
    HAL_Delay(50);
    uint8_t cmd[2] = {0x20, 0x03};
    HAL_I2C_Master_Transmit(i2c_line, SGP30_ADDRESS, cmd, 2, 1000);
    HAL_Delay(10);
}

bool SGP30_read(uint16_t *co2, uint16_t *tvoc) {
    uint8_t cmd[] = {0x20, 0x03};
    if (HAL_I2C_Master_Transmit(i2c_line, SGP30_ADDRESS, cmd, 2, 1000) != HAL_OK)
        return false; 
    HAL_Delay(10);

    uint8_t data[6];
    if (HAL_I2C_Master_Receive(i2c_line, SGP30_ADDRESS, data, 6, 1000) != HAL_OK)
        return false; 
    
    if (sensiron_crc8(data, 2) != data[2] || 
        sensiron_crc8(data + 3, 2) != data[5])
        return false; 


    *co2 = (data[0] << 8) | data[1];
    *tvoc = (data[3] << 8) | data[4];
    return true; 
}


/*
    Integratie in de main.c file:

        #include "co2sensor_stm.h"


        SGP30_init(); // Initialiseer de SGP30 sensor
        uint16_t co2, tvoc; // Variabelen voor CO2 en TVOC waarden

        while (1) {
            if (SGP30_read(&co2, &tvoc)) {
                printf("CO2: %d ppm, TVOC: %d ppb\n", co2, tvoc);
            } else {
                printf("Fout bij het lezen van de sensor\n");
            }
            HAL_Delay(1000); // Wacht 1 seconde voor de volgende meting
        }

*/ 
