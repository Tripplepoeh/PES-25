#ifndef PISOCKET_H
#define PISOCKET_H

#include "socket.h"

#define FIFO_WRITE "/tmp/pi_to_i2cmaster"
#define FIFO_READ  "/tmp/i2cmaster_to_pi"

class PiSocket : public Socket {
private:
    int fifoReadFd = -1;
    int fifoWriteFd = -1;

public:
    PiSocket();
    ~PiSocket();
    void socketInit(const char* ip, int port);
    void handleClient() ;

private:
    void communicationLoop();
    void handleSocketRead();
    void handleFifoRead();
    void sendMessageToFifo(const char* message);
    void sendMessageToPi(const char* message);
};

#endif
