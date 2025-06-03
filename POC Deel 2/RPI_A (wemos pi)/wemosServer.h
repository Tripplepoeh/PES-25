#ifndef WEMOSSOCKET_H
#define WEMOSSOCKET_H

#include "socket.h"
#include "statusControl.h"

// maximum aantal toegestane clients
#define MAXCLIENTS 5

class WemosSocket : public Socket {
private:
    int clientSockets[MAXCLIENTS];  // array van client socket file descriptors
    int sd, max_sd, curSocket = 0;  // huidige socket, maximum socket descriptor, aantal actieve sockets
    bool quit = false;              // flag om server te stoppen
    statuscontrole status;         // statuscontrole object voor sensor en actuator communicatie

public:
    WemosSocket();                 // constructor
    void socketInit(const char* ip, int port);  // initialiseer server socket
    void acceptClient();          // accepteer inkomende client verbindingen
    void handleClient();          // verwerk data van clients
    void cleanup();               // sluit alle verbindingen en maak schoon
};

#endif // WEMOSSOCKET_H
