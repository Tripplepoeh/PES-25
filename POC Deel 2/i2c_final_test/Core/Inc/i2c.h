#ifndef __I2C_H
#define __I2C_H

#include "main.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

extern I2C_HandleTypeDef hi2c1;

#define RX_BUF_SIZE 16
#define TX_BUF_SIZE 16

extern uint8_t rx_buf[RX_BUF_SIZE];
extern uint8_t tx_buf[TX_BUF_SIZE];

void MX_I2C_Init(void);
void ProcessReceivedData(uint8_t *data, uint16_t size);
void PrepareNextTxData(uint8_t *buf,   uint16_t size);

#endif /* __I2C_H */
