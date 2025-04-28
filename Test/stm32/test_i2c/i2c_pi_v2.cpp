// i2c_master_wiringPi.cpp
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <iostream>

// Helper‐klasse voor één I²C-slave via wiringPi
class I2CDevice {
public:
    I2CDevice(uint8_t addr) {
        if (wiringPiSetup() == -1)                                                  // check of wiringPi goed is ingesteld
            throw std::runtime_error("Runtime error: setup gefaald");               // error
        fd = wiringPiI2CSetup(addr);                                                // open I2C-device op dit 7-bit adres
        if (fd < 0)                                                                 // check of fd geldig is              
            throw std::runtime_error("Setup fout voor slave: " + toHex(addr));      // error indien ongeldig fd
    }   

    // schrijf een array van bytes
    void writeData(const std::vector<uint8_t>& data) {                              // schrijf array naar slave
        for (auto b : data) {                                                       // loop door array
            if (wiringPiI2CWrite(fd, b) < 0)                                        // zodra er geen byte meer is
                throw std::runtime_error("I2C write error");                        // error indien fout
        }                                                                           // einde van de loop
    }

    // lees een array van bytes
    std::vector<uint8_t> readData(size_t len) {                                     // lees array van slave
        std::vector<uint8_t> buf(len);                                              // maak een buffer aan van de juiste lengte
        for (size_t i = 0; i < len; ++i) {                                          // loop door de lengte
            int b = wiringPiI2CRead(fd);                                            // lees een byte van de slave
            if (b < 0)  {                                                           // check of er een fout is
                throw std::runtime_error("I2C read error");                         // error indien fout
            buf[i] = static_cast<uint8_t>(b);                                       // zet de byte in de buffer
            }                                                                       // einde van de loop
        }
        return buf;                                                                 // return buffer met de gelezen bytes
    }

private:
    int fd;                                                                         // file descriptor voor I²C-device

    // hulpfunctie om hex-string te maken voor foutmelding
    static std::string toHex(uint8_t v) {
        char buf[5];
        std::snprintf(buf, sizeof(buf), "0x%02X", v);
        return std::string(buf);
    }
};

int main() {
    try {
        I2CDevice dev30(0x30);                                                      // maak een I²C-slave aan met adres 0x30
        I2CDevice dev31(0x31);                                                      // maak een I²C-slave aan met adres 0x31

        std::vector<uint8_t> out30 = {0x10, 0x20, 0x30};                            // maak een array aan met data om te versturen naar slave 0x30
        dev30.writeData(out30);                                                     // schrijf de data naar de slave 0x30

        usleep(10000);                                                              // wacht 10ms om de slave tijd te geven om te reageren

        auto in30 = dev30.readData(16);                                             // lees 16 bytes van de slave 0x30
        std::cout << "Slave 0x30 zei:";                                             // print de tekst
        for (auto b : in30) std::cout << " " << std::hex << int(b);                 // loop door de gelezen bytes en print ze in hex
        std::cout << std::endl;                                                     // sluit print af met nieuwe regel

        std::vector<uint8_t> out31 = {0xAA, 0xBB, 0xCC};                            // zelfde voor slave 0x31
        dev31.writeData(out31);
        usleep(10000);
        auto in31 = dev31.readData(16);
        std::cout << "Slave 0x31 zei:";
        for (auto b : in31) std::cout << " " << std::hex << int(b);
        std::cout << std::endl;
    }
    catch (const std::exception& ex) {
        std::cerr << "Fout: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
