#ifndef PISOCKET_H
#define PISOCKET_H

#include "Socket.h"
#include <fcntl.h> // open, O_RDONLY, etc.
#include <sys/stat.h> // mkfifo
#include <unistd.h>
#include <cstring>
#include <iostream>

class PiSocket : public Socket {
private:
    int fifo_fd;
    int new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char recvBuffer[512];
    char sendBuffer[512];

public:
    PiSocket();
    ~PiSocket();

    void run();

private:
    void init();
    void handleConnections();
    void handleSocketRead();
    void handleFifoRead();
    void sendMessage(const char* message);
};

#endif 
