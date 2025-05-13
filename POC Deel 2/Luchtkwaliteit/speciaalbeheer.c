/*
matrix display voor het weergeven van de luchtkwaliteitsindex
versie 1.0
09-05-2025
@Tripplepoeh
*/ 

extern SPI_HandleTypeDef hspi1;
#define LOAD_Pin  GPIO_PIN_4 // Pin 4 van GPIOA
#define LOAD_GPIO_Port GPIOA // GPIOA

const uint8_t fontDigits[10][8] = { 
    {0x3C,0x66,0x6E,0x76,0x66,0x66,0x3C,0x00}, // 0
    {0x18,0x38,0x18,0x18,0x18,0x18,0x3C,0x00}, // 1
    {0x3C,0x66,0x06,0x0C,0x30,0x60,0x7E,0x00}, // 2
    {0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00}, // 3
    {0x0C,0x1C,0x3C,0x6C,0x7E,0x0C,0x0C,0x00}, // 4
    {0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00}, // 5
    {0x1C,0x30,0x60,0x7C,0x66,0x66,0x3C,0x00}, // 6
    {0x7E,0x66,0x0C,0x18,0x18,0x18,0x18,0x00}, // 7
    {0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00}, // 8
    {0x3C,0x66,0x66,0x3E,0x06,0x0C,0x38,0x00}  // 9
  };

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

// Functie om een cijfer te scrollen op de matrix 

uint8_t displayBuffer[8] = {0}; // 8 rijen

void MAX7219_UpdateMatrix(uint8_t *buffer) {
    for (uint8_t row = 0; row < 8; row++) {
        MATRIX_SEND(row + 1, buffer[row]);
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


/*
main code voor in de main.c

int main(void) {
    MATRIX_Init(); // Initialiseer de matrix
    while (1) {
        for (uint8_t i = 0; i < 10; i++) {
            scrollDigit(i); // Scroll elk cijfer van 0 tot 9
        }
    }
*/