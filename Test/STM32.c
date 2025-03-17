// code werkt niet op zichzelf maar moet in de IDE geplaatst worden

#include "main.h"
#include <string.h>

I2C_HandleTypeDef hi2c1;

#define BUFFERSIZE 20
uint8_t i2c_buffer[BUFFERSIZE];
#define LED_PIN GPIO_PIN_5
#define LED_PORT GPIOA

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);

int main(void){
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    
    // stm32 in "listen" mode op i2c
    // in HAL moet je vaak interrupt of DMA gebruiken
    
    while (1){
        memset(i2c_buffer, 0, BUFFERSIZE);

        if (HAL_I2C_Slave_Receive(&hi2c1, i2c_buffer, BUFFERSIZE, HAL_MAX_DELAY) == HAL_OK) {
            if (strstr((char*)i2c_buffer, "LED_ON") != NULL) {
                HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET); // LED aan
            }
            
            else if (strstr((char*)i2c_buffer, "LED_OFF") != NULL) {
                HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET); // LED uit
            }
        }
    }

}