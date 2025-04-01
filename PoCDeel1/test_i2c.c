#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define SLAVE_ADDRESS 0x08 

int main() {
  
    int fd = wiringPiI2CSetup(SLAVE_ADDRESS);
    if (fd == -1) {
        printf("Failed to init I2C communication.\n");
        return -1;
    }
    printf("I2C communication successfully setup.\n");

    const char *fifo_path = "/tmp/test_fifo";
    if (access(fifo_path, F_OK) == -1) {
        if (mkfifo(fifo_path, 0666) == -1) {
            perror("Failed to create FIFO");
            return -1;
        }
        printf("FIFO created.\n");
    }
    
    
    int fs = open(fifo_path, O_RDONLY | O_NONBLOCK);
    if (fs == -1) {
        perror("Failed to open FIFO for reading");
        return -1;
    }

    static int teller = 0;

    while (1) {
        uint8_t data_to_send = 1;  

        // Read from FIFO
        char msg[512];
        int n = read(fs, msg, sizeof(msg) - 1);
        if (n > 0) {
            msg[n] = '\0';  
            printf("FIFO Message: %s\n", msg);
            
                wiringPiI2CWrite(fd, data_to_send);  
                printf("Data sent: %d\n", data_to_send);
        }

       
        int received_data = wiringPiI2CRead(fd);  
        printf("Data received: %d\n", received_data);

        
        if (received_data == 1) {
            printf("Deur geopend!\n");
        } else {
            printf("Deur gesloten.\n");
        }

        usleep(30000);  
    }

    close(fd);  
    close(fs); 
    return 0;
}
