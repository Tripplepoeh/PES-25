/*
matrix display voor het weergeven van de luchtkwaliteitsindex
versie 1.0
09-05-2025
@Tripplepoeh
*/ 

extern SPI_HandleTypeDef hspi1;
#define LOAD_Pin  GPIO_PIN_4 // Pin 4 van GPIOA
#define LOAD_GPIO_Port GPIOA // GPIOA

void MATRIX_SEND(uint8_t add, uint8_t data) {
    uint8_t buffer[2]; = {add, data}; // Buffer voor adres en de data

    HAL_GPIO_WritePin(LOAD_GPIO_Port, LOAD_Pin, GPIO_PIN_RESET); // Zet LOAD laag
    HAL_SPI_Transmit(&hspi1, &add, 1, HAL_MAX_DELAY); // Verstuur het adres
    HAL_GPIO_WritePin(LOAD_GPIO_Port, LOAD_Pin, GPIO_PIN_SET); // Zet LOAD hoog
}

void MATRIX_Init() {
    MATRIX_SEND(0x0F, 0x00);
    MATRIX_SEND(0x0C, 0x01);
    MATRIX_SEND(0x0B, 0x07);
    MATRIX_SEND(0x0A, 0x08);
    MATRIX_SEND(0x09, 0x00);
    for (int i = 0; i <= 8; i++) {
        MATRIX_SEND(i, 0x00); // Zet alle rijen uit
    }
}

