#include "I2C.h"
#include <iostream>
#include <vector>
#include <unistd.h>  // voor usleep

int main() {
    I2CMaster i2c;

    std::vector<int> slaveAddresses = { 0x12 };

    if (!i2c.init(slaveAddresses)) {
        std::cerr << "I2C initialisatie mislukt!\n";
        return 1;
    }

    while (true) {
        i2c.pollSlaves();
        usleep(10000);  // 10ms delay
    }

    return 0;
}
