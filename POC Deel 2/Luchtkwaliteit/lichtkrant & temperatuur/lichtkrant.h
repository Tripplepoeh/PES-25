#ifndef LICHTKRANT_H
#define LICHTKRANT_H

#include <stdint.h>

#define NUM_MODULES 4
#define LOAD_Pin GPIO_PIN_4
#define LOAD_GPIO_Port GPIOA

void Lichtkrant_SendAll(uint8_t address, uint8_t data);
void Lichtkrant_Initcascade(void);

// Framebuffer update functies
extern uint8_t Lichtkrant_Framebuffer[NUM_MODULES*8][8];
void Lichtkrant_UpdateFrame(void);

void buildScrollBufferFromString(const char *text, uint8_t *buffer, int *length);

void scrollBuffer32(uint8_t *cols, int len, uint16_t delay_ms);

#endif // LICHTKRANT_H