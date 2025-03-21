#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};


    // socket aanmaken
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    // socket opties
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // binden
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // luisteren
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    // accepteer verbinding

    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    if (new_socket < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    std::cout << "New connection accepted" << std::endl;

    while(true) {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(new_socket, buffer, 1024);
        if (valread > 0) {
            std::cout << "Message received: " << buffer << std::endl;
            // hier iets doen met de data 
            // bijvoorbeeld doorsturen naar een ander programma (online voorbeeld gebruikt)	
            system(("./ServerSend_Pi2 " + std::string(buffer)).c_str());
            }
        else {
            std::cout << "Client disconnected" << std::endl;
            break;
        }
    close(new_socket);
    close(server_fd);
    return 0;
    }
}