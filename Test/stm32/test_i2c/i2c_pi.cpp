// master code voor testen i2c en mogelijke implementatie

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include "i2c_pi.h"

int main() {
    try {
        I2CMaster bus;

        bus.setSlaveadd(0x30); // voorbeeld adres, pas aan naar behoefte


        // data schrijven naar slave
        std::vector<uint8_t> exData = {0x01, 0x02, 0x03}; // voorbeeld data
        bus.writeData(exData);

        std::cout << "Data geschreven: ";
        for (auto byte : exData) {
            std::cout << std::hex << (int)byte << " ";
        }
        std::cout << std::endl;

        usleep (1000); //wacht 1ms (aanpassen naar behoefte) :)

        // data lezen van slave
        auto ontData = bus.readData(16); // voorbeeld lengte, pas aan naar behoefte
        std::cout << "Data gelezen: ";
        for (auto byte : ontData) {
            std::cout << std::hex << (int)byte << " ";
        }
        std::cout << std::endl;

        // bus.setSlaveadd(0x31); volgende slave, hele zooitje opnieuw doen en kassa

    } 
    catch (const std::exception& e) {
        std::cerr << "Fout: " << e.what() << std::endl;
    }
    return 0;
    
}