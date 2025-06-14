#include "PiSocket.h"
#include "socket.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <algorithm>

// Constructor: Initialiseert een PiSocket object
PiSocket::PiSocket() {}

// Destructor: Sluit open file descriptors en verwijdert FIFO pipes
PiSocket::~PiSocket() {
    if (fifoReadFd >= 0) close(fifoReadFd);
    if (fifoWriteFd >= 0) close(fifoWriteFd);
    if (new_socket >= 0) close(new_socket);
    unlink(FIFO_READ);
    unlink(FIFO_WRITE);
}

// Initialiseer socket verbinding en FIFO pipes
void PiSocket::socketInit(const char* ip, int port) {
    // Maak een client socket aan
    new_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (new_socket < 0) { perror("socket failed"); exit(EXIT_FAILURE); }

    // Stel adres en poort in
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &address.sin_addr) <= 0) {
        perror("Invalid address"); exit(EXIT_FAILURE);
    }

    // Maak FIFO pipes aan voor communicatie met andere processen
    mkfifo(FIFO_WRITE, 0666);
    mkfifo(FIFO_READ, 0666);
    // Open FIFO voor lezen (blokkerend tot andere kant open is)
    while ((fifoReadFd = open(FIFO_READ, O_RDONLY | O_NONBLOCK)) < 0) {
        usleep(100000);
    }
    // Open FIFO voor schrijven (blokkerend tot andere kant open is)
    while ((fifoWriteFd = open(FIFO_WRITE, O_WRONLY | O_NONBLOCK)) < 0) {
        usleep(100000);
    }

    // Verbind met de server
    std::cout << "PiSocket: connecting to " << ip << ":" << port << std::endl;
    while (connect(new_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("connect failed, retrying");
        sleep(1);
    }
    std::cout << "PiSocket: connected to server" << std::endl;

    std::cout << "PiSocket: FIFO initialized" << std::endl;
}

// Start de communicatie loop met de server en de FIFO
void PiSocket::handleClient() {
    communicationLoop();
}

// Hoofd communicatielus: selecteert tussen socket en FIFO input
void PiSocket::communicationLoop() {
    fd_set readfds;
    int max_fd = std::max(new_socket, fifoReadFd);
    while (new_socket >= 0) {
        FD_ZERO(&readfds);
        FD_SET(new_socket, &readfds);   // Voeg socket toe aan set
        FD_SET(fifoReadFd, &readfds);   // Voeg FIFO toe aan set
        int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) { perror("select error"); break; }
        // Als er data binnenkomt op de socket, verwerk deze
        if (FD_ISSET(new_socket, &readfds)) handleSocketRead();
        // Als er data binnenkomt op de FIFO, verwerk deze
        if (FD_ISSET(fifoReadFd, &readfds)) handleFifoRead();
    }
}

// Lees data van de socket en stuur deze door naar de FIFO
void PiSocket::handleSocketRead() {
    char buf[512];
    int len = read(new_socket, buf, sizeof(buf)-1);
    if (len <= 0) {
        std::cout << "PiSocket: server disconnected" << std::endl;
        close(new_socket);
        new_socket = -1;
    } else {
        buf[len] = '\0';
        std::cout << "PiSocket: Received from server: " << buf;
        // Stuur het bericht door naar de FIFO pipe
        sendMessageToFifo(buf);
    }
}

// Lees data van de FIFO en stuur deze door naar de socket
void PiSocket::handleFifoRead() {
    char buf[512];
    ssize_t n = read(fifoReadFd, buf, sizeof(buf)-1);
    if (n > 0) {
        buf[n] = '\0';
        std::cout << "PiSocket: Received from FIFO: " << buf;
        // Stuur het bericht door naar de Pi via socket
        sendMessageToPi(buf);
    }
}

// Stuur een bericht naar de FIFO pipe
void PiSocket::sendMessageToFifo(const char* message) {
    if (fifoWriteFd >= 0 && *message) {
        write(fifoWriteFd, message, strlen(message));
        std::cout << "PiSocket: sent to FIFO: " << message;
    }
}

// Stuur een bericht naar de Pi via de socket
void PiSocket::sendMessageToPi(const char* message) {
    if (new_socket >= 0 && *message) {
        send(new_socket, message, strlen(message), 0);
        std::cout << "PiSocket: sent to client: " << message;
    }
}
