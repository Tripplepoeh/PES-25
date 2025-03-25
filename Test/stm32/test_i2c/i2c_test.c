//
// in de main.c main func:
//
//     if(HAL_I2C_Enablelisten_IT(&hi2c1) != HAL_OK)
//     {
//         Error_Handler();
//     }
//
#include "main.h"
#include <string.h>
#include "slave2.h"

extern I2C_HandleTypeDef hi2c1;

#define SIZE 32 //buffer size 32 bytes

uint8_t rxData[SIZE]; //buffer to store received data
uint8_t txData[SIZE]; //buffer to store data to be sent

uint8_t rxcount = 0; //counter to keep track of received data
uint8_t txcount = 0; //counter to keep track of data to be sent

uint8_t firstByte = 0;

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
    if(TransferDirection == I2C_DIRECTION_TRANSMIT)
    {
        //Master is sending data to slave
        //[len, data1, data2, data3, ...]
        //len = number of bytes to be sent
        if(firstByte == 0)
        {
            rxcount=0;
            HAL_I2C_Slave_Seq_Receive_IT(&hi2c1, rxData+rxcount, 1, I2C_FIRST_FRAME);
        }
    }
    else
    {//Master is reqesting data
        //Slave has to send data to master
        //txData[0] = number of bytes to be sent
        //txData[1] = data1
        //txData[2] = data2
        //txData[3] = data3
        //...
        txData[0] = 1; //number of bytes to be sent || koppelen aan output waarde sensor
        txData[1] = 0x01; //data byte 1 || koppelen aan output waarde sensor
        //txData[2] = 0x01; //data byte 2 || koppelen aan output waarde sensor
        //txData[3] = 0x01; //data byte 3 || koppelen aan output waarde sensor
        //txData[4] = 0x00; //data byte 4 || koppelen aan output waarde sensor
        HAL_I2C_Slave_Seq_Transmit_IT(&hi2c1, txData, txData[0], I2C_LAST_FRAME);
    }
}


void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(firstByte == 0)
    {
        firstByte = 1;
        rxcount++;
        HAL_I2C_Slave_Seq_Receive_IT(&hi2c1, rxData+rxcount, rxData[0], I2C_LAST_FRAME);
    }
    else
    {
        firstByte = 0;
        rxcount = rxData[0];
    }
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    HAL_I2C_EnableListen_IT(&hi2c1);
}