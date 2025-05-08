/*
 * i2c.c
 *
 *  Created on: May 8, 2025
 *      Author: danie
 */


#include "i2c.h"

extern UART_HandleTypeDef huart2;  // jouw UART-handle in main.c
extern I2C_HandleTypeDef hi2c1;
uint8_t rx_buf[RX_BUF_SIZE];
uint8_t tx_buf[TX_BUF_SIZE];

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c,
                          uint8_t TransferDirection,
                          uint16_t AddrMatchCode)
{
  if (hi2c->Instance == I2C1) {
    // 1) Bepaal het 7-bit-adres (AddrMatchCode is al 7-bit)
    uint8_t addr7 = AddrMatchCode & 0x7F;

    // 2) Stuur het via UART
    char buf[32];
    int len = snprintf(buf, sizeof(buf),
                       "ADDR MATCH at 0x%02X %s\r\n",
                       addr7,
                       (TransferDirection == I2C_DIRECTION_TRANSMIT)
                         ? "(RX)" : "(TX)");
    HAL_UART_Transmit(&huart2, (uint8_t*)buf, len, HAL_MAX_DELAY);

    // 3) Toggle LED om visueel terugkoppeling te houden
    HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    HAL_Delay(50);

    // 4) Zet de juiste slave-operatie op
    if (TransferDirection == I2C_DIRECTION_TRANSMIT) {
      HAL_I2C_Slave_Receive_IT(hi2c, rx_buf, RX_BUF_SIZE);
    } else {
      PrepareNextTxData(tx_buf, TX_BUF_SIZE);
      HAL_I2C_Slave_Transmit_IT(hi2c, tx_buf, TX_BUF_SIZE);
    }
  }
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1) {
        // Zet de slave weer in Address-Match luistermodus
        HAL_I2C_EnableListen_IT(hi2c);
    }
}

//void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
//{
//    if (hi2c->Instance == I2C1) {
//        if (TransferDirection == I2C_DIRECTION_TRANSMIT) {
//            // master -> slave
//            // vul hier de rx_buf met de data die je wilt ontvangen
//            // ...
//            HAL_I2C_Slave_Receive_IT(hi2c, rx_buf, RX_BUF_SIZE);
//        } else {
//
//            // slave -> master
//            // vul hier de tx_buf met de data die je wilt verzenden
//            // bijvoorbeeld: tx_buf[0] = 0x00; tx_buf[1] = 0x01;
//            PrepareNextTxData(tx_buf, TX_BUF_SIZE);
//            HAL_I2C_Slave_Transmit_IT(hi2c, tx_buf, TX_BUF_SIZE);
//        }
//    }
//}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1) {
        ProcessReceivedData(rx_buf, RX_BUF_SIZE); // Schrijf deze functie zelf, clear buffer na verwerken ig?
        HAL_I2C_Slave_Receive_IT(hi2c, rx_buf, RX_BUF_SIZE); // Start opnieuw met ontvangen
    }
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1) {
        // Verzend data naar de master
        HAL_I2C_Slave_Transmit_IT(hi2c, tx_buf, TX_BUF_SIZE); // Start opnieuw met verzenden
    }
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1) {
        HAL_I2C_EnableListen_IT(hi2c); // Herstart de I2C in listen mode
    }
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

void ProcessReceivedData(uint8_t *data, uint16_t size)
{
    char msg[32];
    // Stuur een header over UART
    const char *hdr = "\r\n[I2C SLAVE] RX:\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)hdr, strlen(hdr), HAL_MAX_DELAY);

    // Print elke byte
    for (uint16_t i = 0; i < size; i++)
    {
        int len = snprintf(msg, sizeof(msg), "  byte %2u = 0x%02X\r\n", i, data[i]);
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, len, HAL_MAX_DELAY);
    }
}


void PrepareNextTxData(uint8_t *buf, uint16_t size)
{
    char msg[32];

    // Stuur een header over UART
    const char *hdr = "\r\n[I2C SLAVE] TX:\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)hdr, strlen(hdr), HAL_MAX_DELAY);

    // Vul de buffer met een eenvoudig patroon en print het
    for (uint16_t i = 0; i < size; i++)
    {
        buf[i] = (uint8_t)(i * 3 + 1); // voorbeeldpatroon
        int len = snprintf(msg, sizeof(msg), "  byte %2u = 0x%02X\r\n", i, buf[i]);
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, len, HAL_MAX_DELAY);
    }
}
