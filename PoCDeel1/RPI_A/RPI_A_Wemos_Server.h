#ifndef RPI_A_WEMOS_SERVER_H
#define RPI_A_WEMOS_SERVER_H




#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <algorithm>  // Voor std::remove
#include "statuscontrole.h"


#define MAXCLIENTS 5


#define PORT 12348
#define ServerAdres "145.52.127.184"


  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};
  char respons[50];
  
  statuscontrole status;
   int quit;
   fd_set socketSet;
   int sd, max_sd, curSocket;
   int clientSockets[MAXCLIENTS];
   
   char key[50];
   int value;
   int resultaat;
  void socketInit();
  void handleClient();
  void acceptClient();
  

#endif 
