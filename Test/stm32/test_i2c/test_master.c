#include <wiringPiI2C.h>
#include <stdio.h>
#include <unistd.h>

#define SLAVE_ADDRESS 0x16 // Slave address

int main() {
    int fd;
    int num_bytes;
    unsigned char rxBuffer[32]; // Buffer om data op te slaan

    // Initializeer I2C communicatie
    fd = wiringPiI2CSetup(SLAVE_ADDRESS);
    if (fd == -1) {
        printf("Error, geen I2C communicatie gevonden.\n");
        return -1;
    }
    printf("I2C verbinding succesvol.\n");

    // Verzoek data
    num_bytes = wiringPiI2CRead(fd);
    if (num_bytes == -1) {
        printf("Niet in staat om hoeveelheid bytes te lezen\n");
        return -1;
    }
    printf("Bytes ontvangen: %d\n", num_bytes);

    // Lees data
    for (int i = 0; i < num_bytes; i++) {
        rxBuffer[i] = wiringPiI2CRead(fd);
        if (rxBuffer[i] == -1) {
            printf("Niet in staat om data te lezen %d.\n", i);
            return -1;
        }
    }

    // Print data
    printf("Ontvangen data:\n");
    for (int i = 0; i < num_bytes; i++) {
        printf("Ontvangen byte %d: 0x%02X\n", i, rxBuffer[i]);
    }

    return 0;
}