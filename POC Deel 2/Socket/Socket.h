#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h> // open, O_RDONLY, etc.
#include <sys/stat.h> // mkfifo
#include <cstdio>


class Socket {
protected:	
	int server_fd, new_socket, opt = 1;
    struct sockaddr_in address;
    int addrlen;
    fd_set socketSet;
    char recvBuffer[512];
    char sendBuffer[512];
	
public:
	Socket();
    virtual ~Socket();
    virtual void socketInit(const char* ip, int port) = 0;
	virtual void handleClient() = 0;
};

#endif // SOCKET_H
