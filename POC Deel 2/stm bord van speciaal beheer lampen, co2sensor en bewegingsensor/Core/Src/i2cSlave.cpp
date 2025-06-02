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

//Voeg objecten toe die gebruikt worden in de code
extern UART_HandleTypeDef huart2;
extern I2C_HandleTypeDef hi2c1;
extern I2CBeheer i2c;
//Grootte van binnenkomende en uitgaande berichten
#define RX_BUF_SIZE 32
#define TX_BUF_SIZE 32

int error=0;
//Verzend en ontvangst arrays
uint8_t rx_buf[RX_BUF_SIZE] = {0};
uint8_t rxcount =0;
int firstByte = 0;
char msg[512];

uint8_t tx_buf[TX_BUF_SIZE]  = {0};
uint8_t txcount = 0;

//Interrupt callback wanneer adres wordt aangeroepen met richting van verkeer
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c,
                          uint8_t TransferDirection,
                          uint16_t AddrMatchCode)
{
	if (hi2c->Instance == I2C1) { //Ga hier alleen in wanneer het gaat om de juiste i2c lijn
		//Debug tekst
        char buf[32];
        int len = snprintf(buf, sizeof(buf),
                           "ADDR MATCH at 0x%02X %s\r\n",
						   AddrMatchCode,
                           (TransferDirection == I2C_DIRECTION_TRANSMIT)
                             ? "(RX)" : "(TX)");
        HAL_UART_Transmit(&huart2, (uint8_t*)buf, len, HAL_MAX_DELAY);

        //Reset de binnenkomend bericht elke keer als het adres wordt aangeroepen
        memset(rx_buf, 0, RX_BUF_SIZE);


        // Zet de juiste slave-operatie op
        if (TransferDirection == I2C_DIRECTION_TRANSMIT) {//Als master data wilt sturen
        	if(!firstByte){

        		rxcount =0;
        		//Ontvang de eerste byte van de master
        		HAL_I2C_Slave_Sequential_Receive_IT(hi2c, rx_buf+rxcount, 1, I2C_FIRST_FRAME);
        	}
       } else {//Anders stuur een bericht
    	   //Krijg het bericht van I2CBeheer en kopieer het in tx_buf als de grootte past in de array
        	uint8_t len = i2c.getBerichtLength();
        	if (len <= TX_BUF_SIZE) {
        	    memcpy(tx_buf, i2c.getBericht(), len);
        	    txcount = 0;
        	    HAL_I2C_Slave_Sequential_Transmit_IT(hi2c, tx_buf+txcount, 1, I2C_FIRST_FRAME);
        	}
        }
    }
}


void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) //slave ontvang code
{//Verwerk de resterende bytes van de master
    if (hi2c->Instance == I2C1) {
	if (rxcount < RX_BUF_SIZE - 1) {
	    rxcount++;
	    if(rxcount == RX_BUF_SIZE - 1){
	    	//Ontvangt de laatste byte als de grootte van de bytes 1 kleiner is dan de grootte
	        HAL_I2C_Slave_Sequential_Receive_IT(hi2c, rx_buf+rxcount, 1, I2C_LAST_FRAME);
	    } else {
	    	//Ontvangt de bytes als volgende frame anders
	        HAL_I2C_Slave_Sequential_Receive_IT(hi2c, rx_buf+rxcount, 1, I2C_NEXT_FRAME);
	    }
	} else {
	    // Als er een foutmelding is reset alles en zet de lijn weer aan
	    const char *err = "ERROR: Buffer overflow\r\n";
	    HAL_UART_Transmit(&huart2, (uint8_t*)err, strlen(err), HAL_MAX_DELAY);
	    rxcount = 0;
	    ProcessReceivedData1();
	    HAL_I2C_EnableListen_IT(hi2c);  // herstel luistermodus

	}
	 HAL_I2C_EnableListen_IT(hi2c); // herstel luistermodus

}
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c) //slave verstuur code
{
    if (hi2c->Instance == I2C1) {
        // Verzend data naar de master
    	txcount++;
        HAL_I2C_Slave_Sequential_Transmit_IT(hi2c, tx_buf+txcount, 1, I2C_NEXT_FRAME);
    }

}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1) {
        if (rxcount > 0) {
            ProcessReceivedData1();  // proces de data als er data is ontvangen
        }
        HAL_I2C_EnableListen_IT(hi2c); //Zet luister modus weer aan na het bericht
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

void ProcessReceivedData1() {
	//Debug tekst
	 for (uint16_t i = 0; i < rxcount; i++)
	    {
	        int len = snprintf(msg, sizeof(msg), "  byte %2u = 0x%02X\r\n", i, rx_buf[i]);
	        HAL_UART_Transmit(&huart2, (uint8_t*)msg, len, HAL_MAX_DELAY);
	    }
	 int len = snprintf(msg, sizeof(msg), "\r\n");
	 HAL_UART_Transmit(&huart2, (uint8_t*)msg, len, HAL_MAX_DELAY);
	 i2c.ProcessReceivedData(rx_buf, rxcount); //Laat I2CBeheer de data verwerken
}


