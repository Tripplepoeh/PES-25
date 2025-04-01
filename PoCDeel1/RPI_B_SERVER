//#include "RPI_B_Server.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

#define RASPI2_IP "145.52.127.184" // IP address of Raspberry Pi 2
#define RASPI2_PORT 9999           // Port number of Raspberry Pi 2

int sock;                      // Socket file descriptor
struct sockaddr_in serv_addr;  // Struct for server address
char recvBuffer[512];          // Buffer for received messages
char sendBuffer[512];          // Buffer for sending messages

void init();
void zenden(std::string message);

int main() {
    init();
    std::string message = "Hello from Pi1!";
    zenden(message);
	
    while (true) {
        int valread = read(sock, recvBuffer, sizeof(recvBuffer) - 1);
        
        if (valread < 0) {
            perror("Error receiving message");
            break; 
        }

        if (valread > 0) {
            recvBuffer[valread] = '\0'; 
            std::cout << "Received from Pi2: " << recvBuffer << std::endl;
        }
        int fd = open( "/tmp/test_fifo", O_WRONLY | O_NONBLOCK);
        char msg[512];
        
        sscanf(recvBuffer, "%s %s %s", msg); 
        
        if(strcmp(msg, "Knop") == 0){
			
			
			snprintf(msg, sizeof(msg), "%d", 1);
			write(fd, msg, strlen(msg));
			std::cout << "FIFO" << msg << std::endl;;
			
		}
		close (fd);

		
    }
    

    close(sock);
    return 0;
}

void zenden(std::string message) {
    
    send(sock, message.c_str(), message.length(), 0);
    std::cout << "Sent to Pi2: " << message << std::endl;
}

void init() {
    sock = 0; // Initialize socket
    memset(recvBuffer, 0, sizeof(recvBuffer)); 
    memset(sendBuffer, 0, sizeof(sendBuffer));

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(RASPI2_PORT);

    // Set IP address
    if (inet_pton(AF_INET, RASPI2_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return;
    }

    // Connect to Raspberry Pi 2
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return;
    }

    std::cout << "Connected to Pi2 on port " << RASPI2_PORT << std::endl;
}
