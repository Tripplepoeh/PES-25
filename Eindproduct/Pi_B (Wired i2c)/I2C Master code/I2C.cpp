#include "I2C.h"
#include <wiringPiI2C.h>
#include <unistd.h>
#include <iostream>
#include "statuscontrol.h"

I2CMaster::I2CMaster(istatuscontrole* st) : status(st)  {}

bool I2CMaster::init(const std::vector<int>& slaveAddresses) {
    for (int addr : slaveAddresses) {
        int fd = wiringPiI2CSetup(addr); //Setup alle alle slave adressen
        if (fd == -1) {
            std::cerr << "Kan slave op adres 0x" << std::hex << addr << " niet openen\n";
            return false;
        }
        slaveFds[addr] = fd;
        std::cout << "Slave 0x" << std::hex << addr << " verbonden met fd " << fd << "\n";
    }
    return true;
}

void I2CMaster::pollSlaves() {
      // Verwerk eerst eventuele FIFO-opdrachten
         
    for (const auto& [addr, fd] : slaveFds) { //Ga door alle slave adressen heen om hun data te ontvangen en verwerken
        status->leesFifo();
        // 1. Verzoek om status: 0x03 0x04
        uint8_t request[2] = { 0x03, 0x04 };
        if (write(fd, request, sizeof(request)) == -1) {
            std::cerr << "Fout bij verzenden verzoek naar slave 0x" << std::hex << addr << "\n";
            continue;
        }

        // 2. Respons lezen
        std::vector<uint8_t> buffer(32);
        usleep(11000); // kleine delay om slave kans te geven te reageren
        ssize_t bytesRead = read(fd, buffer.data(), buffer.size());
        if (bytesRead <= 0) {
            std::cerr << "Geen data ontvangen van slave 0x" << std::hex << addr << "\n";
            continue;
        }

        buffer.resize(bytesRead);
        std::cout << "Ontvangen van slave 0x" << std::hex << addr << ": ";
        for (uint8_t b : buffer) std::cout << "0x" << std::hex << (int)b << " ";
        std::cout << std::dec << "\n";

        // 3. Verwerking en actie genereren
            
        status->procesData(buffer.data(), buffer.size());

     
        // Krijg de respons en stuur het naar de slave 
        const std::vector<uint8_t>& response = status->getResponse(); 
        if (!response.empty() && response.size() <= 64) {
            if (write(fd, response.data(), response.size()) == -1) {
                std::cerr << "Fout bij verzenden actie naar slave 0x" << std::hex << addr << "\n";
            } else {
                std::cout << "Actie verzonden naar 0x" << std::hex << addr << ": ";
                for (uint8_t b : response) std::cout << "0x" << std::hex << (int)b << " ";
                std::cout << std::dec << "\n";
            }
        }
    }
     status->stuurBerichtFifo(); //Stuur nieuwe sensorwaardes naar de socket via fifo
}


I2CMaster::~I2CMaster() {
    for (const auto& [_, fd] : slaveFds) { //Sluit alle slave adressen 
        if (fd != -1) close(fd);
    }
}
