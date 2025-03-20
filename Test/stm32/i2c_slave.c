#include "main.h"
#include "i2c_slave.h"

extern I2C_HandleTypeDef hi2c1;

#define RxSIZE 8 // 8 bytes
uint8_t RxBuffer[RxSIZE]; // Buffer for received data
uint8_t rxcount = 0; // Counter for received data

int countAddr = 0; // Counter for address
int countrxcplt = 0; // Counter for RX complete
int countererror = 0; // Counter for error

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
    HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
    if (TransferDirection == I2C_DIRECTION_TRANSMIT) // als master data wil sturen
    {
        rxcount = 0;
        countAddr++;
        //ontvangen sequentiele funcite
        HAL_I2C_Slave_Sequential_Receive_IT(hi2c, RxData+rxcount, 1, I2C_FIRST_FRAME);
    }
    else { // als master data wilt ontvangen
        Error_Handler(); // error
    }
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    // doe iets
} 

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    countererror++;
    uint32_t errorcode = HAL_I2C_GetError(hi2c);
    if (errorcode == 4) {
        // doe iets
    }
}