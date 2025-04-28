
#ifndef WEMOSSOCKET_H
#define WEMOSSOCKET_H

#include "Socket.h"
#include <cstdio>
#include "statuscontrole.h"

#define MAXCLIENTS 5

class WemosSocket : public Socket {
private:
    int clientSockets[MAXCLIENTS];
    int sd, max_sd, curSocket = 0;
    bool quit = false;
    char buffer[1025];
    char key[50];
    int value = 0;
    int resultaat = 0;
    char respons[50];
    statuscontrole status;

public:
    WemosSocket();
    void run();

private:
    void acceptClient();
    void handleClient();
    void cleanup();
};

#endif // WEMOSSOCKET_H
