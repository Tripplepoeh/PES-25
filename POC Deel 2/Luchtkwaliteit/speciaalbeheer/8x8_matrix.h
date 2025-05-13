/*
 * 8x8_matrix.h
 *
 *  Created on: May 13, 2025
 *      Author: @Tripplepoeh
 */

 #ifndef INC_8X8_MATRIX_H_
 #define INC_8X8_MATRIX_H_
 
 #include <stdint.h>
 
 /* --- Defines voor de LOAD (CS) pin --- */
 #define LOAD_Pin         GPIO_PIN_4
 #define LOAD_GPIO_Port   GPIOA
 
 /* --- Functieprototypes --- */
 void MATRIX_SEND(uint8_t add, uint8_t data);
 void MATRIX_Init(void);
 void MAX7219_UpdateMatrix(uint8_t *buffer);
 void buildScrollBufferFromNumber(int number, uint8_t *buffer, int *length);
 void buildScrollBufferFromString(const char *text, uint8_t *buffer, int *length);
 void scrollBuffer(uint8_t *data, int len);
 
 /* --- Externe variabelen --- */
 extern const uint8_t fontDigits[10][8];
 
 #endif /* INC_8X8_MATRIX_H_ */
 