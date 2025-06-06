#ifndef PISOCKET_H
#define PISOCKET_H

#include "socket.h"

#define FIFO_WRITE "/tmp/pi_to_wemosserver"
#define FIFO_READ "/tmp/wemosserver_to_pi"


class PiSocket : public Socket {
private:
    int fifo_fd;
    
    int fifoReadFd = -1;
    int fifoWriteFd = -1;
    	int server_fd, new_socket, opt = 1;
	
    struct sockaddr_in address;
    int addrlen;
    fd_set socketSet;
    char recvBuffer[512];
    char sendBuffer[512];
  

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
