// Menu:
static const char menuDag[] = "Menu van de dag: Pasta Carbonara";


int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init();
    MX_I2C1_Init();

    MAX7219_InitCascade();

    uint8_t scrollCols[256];
    int len;
    float temperatuur, luchtvochrtigheid;
    char buf[128];


    while (1) {
        if (SHt3Xx_ReadTemperatureAndHumidity(&temperatuur, &luchtvochrtigheid) == HAL_OK) {
            int n = snprintf(buf, sizeof(buf), "%s -- Temperatuur: %.1fC, Luchtvochtigheid: %.1f%% ", menuDag, temperatuur, luchtvochrtigheid);
        
        buildScrollBufferFromString(buf, scrollCols, &len);
        scrollBuffer32(scrollCols, len, 100);
        
        } else {
          buildScrollBufferFromString("ERROR! ", scrollCols, &len);
            scrollBuffer32(scrollCols, len, 200);  
        }
    HAL_Delay(500);    
    }

}
