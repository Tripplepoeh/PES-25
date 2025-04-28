#ifndef PISOCKET_H
#define PISOCKET_H

#include "socket.h"

class PiSocket : public Socket {
private:
    int fifo_fd;
  

public:
    PiSocket();
    ~PiSocket();
	
    void socketInit(const char* ip, int port);
	void handleClient();

private:
    void handleConnections();
    void handleSocketRead();
    void handleFifoRead();
	void sendMessageToFifo(const char* message);
    void sendMessageToPi(const char* message);
	
};

#endif 
