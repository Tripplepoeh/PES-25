#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define RASPI2_IP "192.168.137.123" // IP address of the Raspberry Pi 2
#define RASPI2_PORT 9090 // Port number of the Raspberry Pi 2

int main() {
    

    std::string message = "Hoi ik ben sender";

    int sock = 0; // Socket file descriptor
    struct sockaddr_in serv_addr; // Struct for the server address
    char buffer[1024] = {0}; // Buffer for the message

    // socket maken
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket maken mislukt");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(RASPI2_PORT);

    // ip instellen
    if(inet_pton(AF_INET, RASPI2_IP, &serv_addr.sin_addr) <= 0){
        perror("Ongeldig adres");
        return -1;
    }

    // verbinden
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        perror("Verbinden mislukt");
        return -1;
    }

    // bericht sturen
    send(sock, message.c_str(), message.length(), 0);
    std::cout << "naar pi2 gestuurd: " << message << std::endl;

    // bericht ontvangen
    int valread = read(sock, buffer, 1024);
    if (valread < 0) {
        perror("Bericht ontvangen mislukt");
        return -1;
    }
    if (valread >0) {
        std::cout << "van pi2 ontvangen: " << buffer << std::endl;
    }

    close(sock);
    return 0;
}
