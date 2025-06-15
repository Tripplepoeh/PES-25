#include "i2cBeheer.hpp"
#include "alleDefines.h"
#include <vector>
#include "deurknop.h"
#include "deurServoTest.h"
#include <cstdio>

// User variables and objects
I2CBeheer i2c;
deurServotest servo(&htim1);

int main(void)
{
    

  

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    std::vector<uint8_t> Ids = { DEURKNOP, DEUR };
    i2c.I2CInit(Ids, &dk, &servo);
    char buffer[512];

HAL_I2C_EnableListen_IT(&hi2c1);
    while (1)
    {
        dk.poll();
        servo.update();

    }
}
