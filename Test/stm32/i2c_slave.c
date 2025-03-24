#include "main.h" // Include the main header file
#include "i2c_slave.h" // Include the header file
#include "stdio.h"	// For sprintf
#include "stdlib.h"	// For atoi
#include "string.h"	// For memset
#include "servo.h"	// Include the servo header file
#include "button.h"	// Include the button header file

extern I2C_HandleTypeDef hi2c1;

//Register 0: Status van de drukknop (0 = niet ingedrukt, 1 = ingedrukt).
//Register 1: Commando voor de servo (0 = inactief, 1 = activeer).

#define NUM_REG 10 // Number of registers
uint8_t I2C_REGISTERS[NUM_REG] = {0}; // Array for I2C registers

#define RxSIZE 2
uint8_t RxBuffer[RxSIZE]; // Buffer for received data
uint8_t rxcount = 0; // Counter for received data
uint8_t txcount = 0; // Counter for transmitted data
uint8_t startPosition = 0; // Start position for received data
uint8_t bytesRxd = 0; // Number of bytes received
uint8_t bytesTxd = 0; // Number of bytes transmitted


int countAddr = 0; // Counter for address
int countrxcplt = 0; // Counter for RX complete
int countererror = 0; // Counter for error

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if(GPIO_Pin == BUTTON_PIN) { // BUTTON_PIN gedefinieerd in button.h
        I2C_REGISTERS[0] = 1;  // Drukknop ingedrukt: zet register 0 op 1
    }
}

int process_data (void)
{
    int startREG = RxData[0];       // Registeradres waar de data moet worden weggeschreven
    int numREG = bytesRrecvd;         // Aantal ontvangen bytes
    int endREG = startREG + numREG - 1; // Bepaal eindregister

    // Valideer bereik (stel dat registers 0 t/m 9 gebruikt worden)
    if(endREG >= NUM_REGISTERS) {
        memset(RxData, '\0', RxSIZE);
        rxcount = 0;
        return 0;
    }

    int indx = 1; // Data start na het registeradres
    for (int i = 0; i < numREG; i++) {
        I2C_REGISTERS[startREG++] = RxData[indx++];
    }
    
    // Als het commandoregister (bijv. register 1) de waarde 1 heeft gekregen, activeer dan de servo
    if(startREG - numREG <= 1 && I2C_REGISTERS[1] == 1) {
        servo_activate();       // Activeer de servo (deze functie moet je zelf implementeren in servo.c)
        I2C_REGISTERS[1] = 0;   // Reset het commando na verwerking
    }
    return 1;  // Succes
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{


	if (TransferDirection == I2C_DIRECTION_TRANSMIT)  // if the master wants to transmit the data
	{
		RxData[0] = 0;  // reset the RxData[0] to clear any residue address from previous call
		rxcount =0;
		HAL_I2C_Slave_Seq_Receive_IT(hi2c, RxData+rxcount, 1, I2C_FIRST_FRAME);
	}

	else
	{
		txcount = 0;
		startPosition = RxData[0];
		RxData[0] = 0;  // Reset the start register as we have already copied it
		HAL_I2C_Slave_Seq_Transmit_IT(hi2c, I2C_REGISTERS+startPosition+txcount, 1, I2C_FIRST_FRAME);
	}
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	txcount++;
	HAL_I2C_Slave_Seq_Transmit_IT(hi2c, I2C_REGISTERS+startPosition+txcount, 1, I2C_NEXT_FRAME); // Transmit the next byte
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	rxcount++;
	if (rxcount < RxSIZE)
	{
		if (rxcount == RxSIZE-1)
		{
			HAL_I2C_Slave_Seq_Receive_IT(hi2c, RxData+rxcount, 1, I2C_LAST_FRAME);
		}
		else
		{
			HAL_I2C_Slave_Seq_Receive_IT(hi2c, RxData+rxcount, 1, I2C_NEXT_FRAME);
		}
	}

	if (rxcount == RxSIZE)
	{
		process_data();
	}
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	uint32_t errorcode = HAL_I2C_GetError(hi2c);

	if (errorcode == 4)  // AF error
	{
		if (txcount == 0)  // error is while slave is receiving
		{
			bytesRrecvd = rxcount-1;  // the first byte is the register address
			rxcount = 0;  // Reset the rxcount for the next operation
			process_data();
		}
		else // error while slave is transmitting
		{
			bytesTransd = txcount-1;  // the txcount is 1 higher than the actual data transmitted
			txcount = 0;  // Reset the txcount for the next operation
		}
	}

	/* BERR Error commonly occurs during the Direction switch
	 * Here we the software reset bit is set by the HAL error handler
	 * Before resetting this bit, we make sure the I2C lines are released and the bus is free
	 * I am simply reinitializing the I2C to do so
	 */
	else if (errorcode == 1)  // BERR Error
	{
		HAL_I2C_DeInit(hi2c);
		HAL_I2C_Init(hi2c);
		memset(RxData,'\0',RxSIZE);  // reset the Rx buffer
		rxcount =0;  // reset the count
	}

	HAL_I2C_EnableListen_IT(hi2c);
}
