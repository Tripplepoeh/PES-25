#ifndef STATUSCONTROLE_H
#define STATUSCONTROLE_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "RPI_A_Send.h"
#include <fcntl.h>

#define FIFO_NAME "/tmp/my_fifo"

class statuscontrole {
private:
static int vorigeStatus;
int huidigeStatus = 0;
char bericht[512];
//char sendBuffer[512];
//char sendBuffer1[512];
static int fd;  // Add this line

public:
    statuscontrole();  // Constructor
    int welkeWaarde(int drukWaarde);
    void pasBufferAan(char buffer[]);
    void doorsturen(char bericht[]);
    void checkKnop(int drukknop);
    
};

#endif // STATUSCONTROLE_H
