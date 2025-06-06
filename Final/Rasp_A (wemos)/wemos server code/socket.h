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

public:
	Socket();
    virtual ~Socket();
    virtual void socketInit(const char* ip, int port) = 0;
	virtual void handleClient() = 0;
};

#endif // SOCKET_H
