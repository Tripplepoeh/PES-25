#ifndef WEMOSSOCKET_H
#define WEMOSSOCKET_H

#include "socket.h"
//#include "statuscontrole.h"

#define MAXCLIENTS 5

class WemosSocket : public Socket {
private:
    int clientSockets[MAXCLIENTS];
    int sd, max_sd, curSocket = 0;
    bool quit = false;
    //statuscontrole status;

public:
    WemosSocket();
	void socketInit(const char* ip, int port);
	void acceptClient();
    void handleClient();
    void cleanup();

   
};

#endif // WEMOSSOCKET_H
