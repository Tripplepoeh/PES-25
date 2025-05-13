/*
 * 8x8_matrix.c
 *
 *  Created on: May 13, 2025
 *      Author: @Tripplepoeh
 */

 #include "main.h"
 #include <stdio.h>
 
 
 extern SPI_HandleTypeDef hspi1;
 #define LOAD_Pin  GPIO_PIN_4 // Pin 4 van GPIOA
 #define LOAD_GPIO_Port GPIOA // GPIOA
 
 //-=-=-=-=-=-=-=-=-=-=-=-=
 // "CO2: " printen
 
 // extra font voor letters en symbolen
 static const uint8_t fontChars[5][8] = {
   // ' ' (spatie)
   {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
   // 'C'
   {0x7C, 0xFE, 0xC6, 0xC6, 0xC6, 0xEE, 0x6C, 0x00},
   // 'O'
   {0x7C, 0xFE, 0xC6, 0xC6, 0xC6, 0xFE, 0x7C, 0x00},
   // '2'
   { 0x00, 0x46, 0xCE, 0x8A, 0x92, 0xF2, 0x62, 0x00 },
   // ':'
   {0x00, 0x00, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00},
 };
 // mapping van ASCII naar index in fontChars:
 static int charIndex(char c) {
   switch(c) {
     case ' ': return 0;
     case 'C': return 1;
     case 'O': return 2;
     case '2': return 3;
     case ':': return 4;
     default:  // voor veiligheid, val terug op spatie
       return 0;
   }
 }
 
 void buildScrollBufferFromString(const char *text, uint8_t *buffer, int *length) {
   int idx = 0;
   for (int i = 0; text[i] != '\0'; i++) {
     int fi = charIndex(text[i]);
     // voeg 8 kolommen van dit teken toe
     for (int col = 0; col < 8; col++) {
       buffer[idx++] = fontChars[fi][col];
     }
     // 1 kolom lege ruimte tussen tekens
     buffer[idx++] = 0x00;
   }
   *length = idx;
 }
 
 const uint8_t fontDigits[10][8] = {
 /* 0 */ { 0x00, 0x7C, 0xFE, 0x92, 0xA2, 0xFE, 0x7C, 0x00 },
 /* 1 */ { 0x00, 0x00, 0x42, 0xFE, 0xFE, 0x02, 0x00, 0x00 },
 /* 2 */ { 0x00, 0x46, 0xCE, 0x8A, 0x92, 0xF2, 0x62, 0x00 },
 /* 3 */ { 0x00, 0x44, 0xC6, 0x92, 0x92, 0xFE, 0x6C, 0x00 },
 /* 4 */ { 0x00, 0x18, 0x38, 0x68, 0xFE, 0xFE, 0x08, 0x00 },
 /* 5 */ { 0x00, 0xE4, 0xE6, 0xA2, 0xA2, 0xBE, 0x9C, 0x00 },
 /* 6 */ { 0x00, 0x3C, 0x7E, 0xD2, 0x92, 0x9E, 0x0C, 0x00 },
 /* 7 */ { 0x00, 0xC0, 0xC0, 0x9E, 0xBE, 0xE0, 0xC0, 0x00 },
 /* 8 */ { 0x00, 0x6C, 0xFE, 0x92, 0x92, 0xFE, 0x6C, 0x00 },
 /* 9 */ { 0x00, 0x60, 0xF2, 0x92, 0x96, 0xFC, 0x78, 0x00 },
   };
 
 void MATRIX_SEND(uint8_t add, uint8_t data) {
     uint8_t buffer[2] = {add, data}; // Buffer voor adres en de data
 
     HAL_GPIO_WritePin(LOAD_GPIO_Port, LOAD_Pin, GPIO_PIN_RESET);
     HAL_SPI_Transmit(&hspi1, buffer, 2, HAL_MAX_DELAY);
     HAL_GPIO_WritePin(LOAD_GPIO_Port, LOAD_Pin, GPIO_PIN_SET);
 }
 
 void MATRIX_Init() {
     MATRIX_SEND(0x0F, 0x00);  // display test off
     MATRIX_SEND(0x0C, 0x01);  // normal operation
     MATRIX_SEND(0x0B, 0x07);  // scan limit = 8 digits (1–8)
     MATRIX_SEND(0x0A, 0x08);  // medium intensity
     MATRIX_SEND(0x09, 0x00);  // no decode (raw)
 
     for (int i = 0; i <= 8; i++) {
         MATRIX_SEND(i, 0x00); // Zet alle rijen uit
     }
 }
 
 // Functie om een cijfer te scrollen op de matrix
 
 uint8_t displayBuffer[8] = {0}; // 8 rijen
 
 void MAX7219_UpdateMatrix(uint8_t *buffer) {
     uint8_t frame[8] = {0};
 
     // buffer[r] heeft bit7=kolom0 … bit0=kolom7 van rij r
     // we willen nu nieuwe rij i = oorspronkelijke kolom (7-i),
     // en nieuwe kolom j = oorspronkelijke rij j
     for (int row = 0; row < 8; row++) {
         for (int col = 0; col < 8; col++) {
             // check bit in originele buffer: rij=col, kolom=row
             if (buffer[col] & (1 << row)) {
                 // schrijf in frame[row] op bit (7-col) voor horizontale spiegel
                 frame[row] |= (1 << (7 - col));
             }
         }
     }
 
     // stuur naar MAX7219: register 1…8 zijn rijen
     for (uint8_t i = 0; i < 8; i++) {
         MATRIX_SEND(i+1, frame[i]);
     }
 }
 
 void buildScrollBufferFromNumber(int number, uint8_t *buffer, int *length) {
     char str[10];
     sprintf(str, "%d", number); // zet getal om naar string
 
     int idx = 0;
     for (int i = 0; str[i] != '\0'; i++) {
         int digit = str[i] - '0';
         for (int j = 0; j < 8; j++) {
             buffer[idx++] = fontDigits[digit][j];
         }
         // spatie tussen cijfers
         buffer[idx++] = 0x00;
     }
 
     *length = idx;
 }
 
 void scrollBuffer(uint8_t *data, int len) {
     uint8_t display[8] = {0};
 
     for (int i = 0; i < len; i++) {
         for (int row = 0; row < 8; row++) {
             display[row] <<= 1;
             if (data[i] & (1 << (7 - row))) {
                 display[row] |= 1;
             }
         }
         MAX7219_UpdateMatrix(display);
         HAL_Delay(100);
     }
 
     // optioneel: na scrollen display wissen
     for (int row = 0; row < 8; row++) {
         display[row] = 0;
     }
     MAX7219_UpdateMatrix(display);
 }
 