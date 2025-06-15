#include <stdio.h>
#include <string.h>
#include <vector>

#include "lichtkrant.h"
extern "C" {
    #include "SHT3X.h"
}

#include "i2cBeheer.h"
#include "alleDefines.h"
#include "buzzerknop.h"
#include "buzzer.h"

// User variables
char menuDag[32];
uint16_t co2Waarde;

I2CBeheer i2c;
Buzzerknop knop;
Buzzer buzzer(&htim1, TIM_CHANNEL_1, 392);  // 392 Hz



int main(void)
{
    Lichtkrant_Initcascade();

    // Zet display-test definitief uit en normal operation
    Lichtkrant_SendAll(0x0F, 0x00);  // test off
    Lichtkrant_SendAll(0x0C, 0x01);  // normal op
    // Clear rijen 1 t/m 8
    for (uint8_t r = 1; r <= 8; r++) {
        Lichtkrant_SendAll(r, 0x00);
    }
    HAL_Delay(500);

    // Stuur nu slechts rij 1 vol (alle LEDs aan) als test
    Lichtkrant_SendAll(1, 0xFF);
    for (uint8_t r = 2; r <= 8; r++) {
        Lichtkrant_SendAll(r, 0x00);
    }

    uint8_t scrollCols[256];
    int len;
    float temperatuur = 0.0, luchtvochtigheid = 0.0;
    char buf[512];
    char tempStr[8], humStr[8];
    floatToStr1(temperatuur, tempStr);
    floatToStr1(luchtvochtigheid, humStr);
    buildScrollBufferFromString("    Welkom gebruiker", scrollCols, &len);
    scrollBuffer32(scrollCols, len, 45);

    std::vector<uint8_t> Ids = { TEMPSENSOR, LUCHTVSENSOR, BUZZER, LICHTKRANT, SPECIALBEHEERDISPLAY };
    i2c.I2CInit(Ids, &buzzer);
    HAL_TIM_Base_Start_IT(&htim1);
    HAL_I2C_EnableListen_IT(&hi2c1);

    while (1)
    {
        HAL_UART_Transmit(&huart2, (uint8_t*)menuDag, sizeof menuDag, HAL_MAX_DELAY);

        if (SHT3X_Read(&hi2c3, &temperatuur, &luchtvochtigheid) == HAL_OK) {
            floatToStr1(temperatuur, tempStr);
            floatToStr1(luchtvochtigheid, humStr);
            sprintf(buf, "    %s -- Temp: %sC, LV: %s%% -- CO2: %d", menuDag, tempStr, humStr, co2Waarde);
            buildScrollBufferFromString(buf, scrollCols, &len);
            scrollBuffer32(scrollCols, len, 50);
        }
    }
}

void floatToStr1(float f, char *buf) {
    int whole = (int)f;
    int frac  = (int)((f - whole) * 10.0f);
    if (frac < 0) frac = -frac;
    sprintf(buf, "%d.%d", whole, frac);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPIO_PIN_11) {
       knop.gedrukt();
       if(knop.isIngedrukt()){
           buzzer.knopAan();
           HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
       } else {
           buzzer.knopUit();
           HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
       }
    }
}

void setMenuDag(const char* nieuwMenu) {
    strncpy(menuDag, nieuwMenu, sizeof(menuDag) - 1);
    menuDag[sizeof(menuDag) - 1] = '\0'; // Zorg dat het correct afgesloten is
    HAL_UART_Transmit(&huart2, (uint8_t*)menuDag, sizeof menuDag, HAL_MAX_DELAY);
}

void setCO2Waarde(uint16_t nieuweCo2Waarde) {
    co2Waarde = nieuweCo2Waarde;
}
