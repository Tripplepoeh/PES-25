#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>

#define PORT 12348
#define PORT1 9999 // Port number of the Raspberry Pi 2 for the first server
#define ServerAdres "145.52.127.184"
#define FIFO_NAME "/tmp/my_fifo"

class Socket {
protected:
    int server_fd, opt = 1;
    struct sockaddr_in address;
    fd_set socketSet;

public:
    virtual ~Socket() {}
    void socketInit();
};

#endif // SOCKET_H
