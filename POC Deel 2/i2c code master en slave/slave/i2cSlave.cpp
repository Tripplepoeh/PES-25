/*
 * i2cSlave.c
 *
 *  Created on: May 9, 2025
 *      Author: aashi
 */
#include "main.h"

#include "i2cSlave.h"
#include <stdio.h>
#include <string.h>
#include "i2cBeheer.hpp"


extern UART_HandleTypeDef huart2;
extern I2C_HandleTypeDef hi2c1;
extern I2CBeheer i2c;

#define RX_BUF_SIZE 32
#define TX_BUF_SIZE 32

int error=0;

uint8_t rx_buf[RX_BUF_SIZE] = {0};
uint8_t rxcount =0;
int firstByte = 0;

uint8_t tx_buf[TX_BUF_SIZE]  = {0};
uint8_t txcount = 0;

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c,
                          uint8_t TransferDirection,
                          uint16_t AddrMatchCode)
{
	if (hi2c->Instance == I2C1) {

        char buf[32];
        int len = snprintf(buf, sizeof(buf),
                           "ADDR MATCH at 0x%02X %s\r\n",
						   AddrMatchCode,
                           (TransferDirection == I2C_DIRECTION_TRANSMIT)
                             ? "(RX)" : "(TX)");
        HAL_UART_Transmit(&huart2, (uint8_t*)buf, len, HAL_MAX_DELAY);

        // 3) Toggle LED om visueel terugkoppeling te houden
       // HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
        //HAL_Delay(50);
        memset(rx_buf, 0, RX_BUF_SIZE);
        //memset(tx_buf, 0, TX_BUF_SIZE);


        // 4) Zet de juiste slave-operatie op
        if (TransferDirection == I2C_DIRECTION_TRANSMIT) {
        	if(!firstByte){

        		rxcount =0;
        		HAL_I2C_Slave_Sequential_Receive_IT(hi2c, rx_buf+rxcount, 1, I2C_FIRST_FRAME);
        	}
        	//HAL_I2C_Slave_Receive_IT(hi2c, rx_buf, RX_BUF_SIZE);
        } else {
            //PrepareNextTxData(tx_buf, TX_BUF_SIZE);
        	uint8_t len = i2c.getBerichtLength();
        	if (len <= TX_BUF_SIZE) {
        	    memcpy(tx_buf, i2c.getBericht(), len);
        	    txcount = 0;
            //HAL_I2C_Slave_Sequential_Transmit_IT(hi2c, tx_buf, 5, I2C_FIRST_AND_LAST_FRAME);
        	    HAL_I2C_Slave_Sequential_Transmit_IT(hi2c, tx_buf+txcount, 1, I2C_FIRST_FRAME);
        	//HAL_I2C_Slave_Transmit_IT(hi2c, tx_buf, TX_BUF_SIZE);
        	}
        }
    }
}


void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) //slave ontvang code
{
    if (hi2c->Instance == I2C1) {
	if (rxcount < RX_BUF_SIZE - 1) {
	    rxcount++;
	    if(rxcount == RX_BUF_SIZE - 1){
	        HAL_I2C_Slave_Sequential_Receive_IT(hi2c, rx_buf+rxcount, 1, I2C_LAST_FRAME);
	    } else {
	        HAL_I2C_Slave_Sequential_Receive_IT(hi2c, rx_buf+rxcount, 1, I2C_NEXT_FRAME);
	    }
	} else {
	    // Foutmelding + reset
	    const char *err = "ERROR: Buffer overflow\r\n";
	    HAL_UART_Transmit(&huart2, (uint8_t*)err, strlen(err), HAL_MAX_DELAY);
	    rxcount = 0;
	    ProcessReceivedData1();
	    HAL_I2C_EnableListen_IT(hi2c);  // herstel luistermodus

	}
	 HAL_I2C_EnableListen_IT(hi2c);

}
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c) //slave verstuur code
{
    if (hi2c->Instance == I2C1) {
        // Verzend data naar de master
    	txcount++;
        HAL_I2C_Slave_Sequential_Transmit_IT(hi2c, tx_buf+txcount, 1, I2C_NEXT_FRAME);
        //HAL_I2C_EnableListen_IT(hi2c);
    }

}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1) {
        if (rxcount > 0) {
            ProcessReceivedData1();  // VOEG TOE
        }
        HAL_I2C_EnableListen_IT(hi2c);
    }
}
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) //error met i2c
{

    if (hi2c->Instance == I2C1) {
    	error++;
    	uint32_t errorcode = HAL_I2C_GetError(hi2c);
    		if(errorcode == 4){ // als de error een ack error is, blijf verder gaan
    			__HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);
    			if(txcount == 0){
    			ProcessReceivedData1();
    			}else{
    				txcount = txcount -1;
    			}
    		}
        HAL_I2C_EnableListen_IT(hi2c); // Herstart de I2C in listen mode bij een fout
    }
}

void ProcessReceivedData1() { //niks speciaals voor nu leeg
	char msg[32];
	 for (uint16_t i = 0; i < rxcount; i++)
	    {
	        int len = snprintf(msg, sizeof(msg), "  byte %2u = 0x%02X\r\n", i, rx_buf[i]);
	        HAL_UART_Transmit(&huart2, (uint8_t*)msg, len, HAL_MAX_DELAY);
	    }
	 int len = snprintf(msg, sizeof(msg), "\r\n");
	 HAL_UART_Transmit(&huart2, (uint8_t*)msg, len, HAL_MAX_DELAY);
	 i2c.ProcessReceivedData(rx_buf, rxcount);
}


