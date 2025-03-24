#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define PORT 9090
#define I2C_DEV "/dev/i2c-1" // default op pi
#define SLAVE_ADDR 0x00 // I2C slave address

int main () {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // maken socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // instellen socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("192.168.137.123");
    address.sin_port = htons(PORT);

    // binden socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // luisteren
    if (listen(server_fd, 3) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "server luistert op poort: " << PORT << std::endl;

    // accepteren verbinding
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0){
        perror("accept");
        exit(EXIT_FAILURE);
    }
    std::cout << "verbinding geaccepteerd" << std::endl;

 //   // I2C openen
 /*/   int i2c_fd = open(I2C_DEV, O_RDWR);
    if (i2c_fd < 0){
        perror("open i2c device");
        return -1;
    }

    // STM32 selecteren als slave
    if (ioctl(i2c_fd, I2C_SLAVE, SLAVE_ADDR) < 0){
        perror("I2C set slave address");
        return -1;
    }
    /*/
    std::string msg;
    
    while(true){
        memset(buffer, 0, sizeof(buffer));
        int valread = read(new_socket, buffer, 1024);
        if (valread > 0 ){
            msg = buffer;
            std::cout << "ontvangen: " << msg << std::endl;
        
    

//        // I2C write als we PRESSED ontvangen van wmosclient
//        if (msg.find("PRESSED") != std::string::npos){
//            char cmd[] = "LED_ON";
  //          if (write(i2c_fd, cmd, sizeof(cmd)) != (int)sizeof(cmd)) {
    //            perror("schrijf naar i2c");
//
  //          } else {
    //            std::cout << "LED aan verzonden naar STM32" << std::endl;
      //      }
         }
    }
    //close(i2c_fd);
    close(new_socket);
    close(server_fd);
    return 0;
}
