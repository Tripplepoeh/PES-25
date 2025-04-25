#include "i2c.h"

I2C_HandleTypeDef hi2c1;
uint8_t rx_buf[RX_BUF_SIZE];
uint8_t tx_buf[TX_BUF_SIZE];

// void MX_I2C1_Init(void)
// {
//     hi2c1.Instance = I2C1;
//     hi2c1.Init.Timing = 0x00A02E8A; // 100kHz @ 80MHz
//     hi2c1.Init.OwnAddress1 = 0;
//     hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//     hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//     hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
//     hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//     hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//     HAL_I2C_Init(&hi2c1);
// }

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
    if (hi2c->Instance == I2C1) {
        if (TransferDirection == I2C_DIRECTION_TRANSMIT) {
            // master -> slave
            // vul hier de rx_buf met de data die je wilt ontvangen
            // ...

            HAL_I2C_Slave_Receive_IT(hi2c, rx_buf, RX_BUF_SIZE);
        } else {

            // slave -> master
            // vul hier de tx_buf met de data die je wilt verzenden
            // bijvoorbeeld: tx_buf[0] = 0x00; tx_buf[1] = 0x01; 

            HAL_I2C_Slave_Transmit_IT(hi2c, tx_buf, TX_BUF_SIZE);
        }
    }
}

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
        HAL_I2C_EnableListen(hi2c); // Herstart de I2C in listen mode
    }
}