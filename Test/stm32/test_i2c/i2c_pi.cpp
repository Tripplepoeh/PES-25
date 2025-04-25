// master code voor testen i2c en mogelijke implementatie

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <iostream>

class I2CMaster {
public:
    I2CMaster(const char* bus = "/dev/i2c-1"){
        fd = open(bus, O_RDWR);
        if (fd < 0) throw std::runtime_error("I2C bus niet gevonden");
    }

    ~I2CMaster(){ if (fd >= 0) close(fd);
    }

    void setSlaveadd(uint8_t addr){
        if (ioctl(fd, I2C_SLAVE, addr) < 0) throw std::runtime_error("I2C adress niet geregistreerd");
    }

    void writeData(const std::vector<uint8_t>& data){
        if (::write(fd, data.data(), data.size()) != (ssize_t)data.size())
            throw std::runtime_error("I2C write error");
    }

    std::vector<uint8_t> readData(size_t len){
        std::vector<uint8_t> buf(len);
        if (::read(fd, buf.data(), len) != (ssize_t)len)
            throw std::runtime_error("I2C read error");
        return buf;
    }

private:
    int fd;
};


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