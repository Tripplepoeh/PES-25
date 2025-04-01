#ifndef RPI_A_Send_H
#define RPI_A_Send_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <algorithm>  // Voor std::remove


#include <fcntl.h>

#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <sys/stat.h>  // Vereist voor mkfifo()



#define ServerAdres1 "145.52.127.184" // IP address of the Raspberry Pi 2 for the first server
#define PORT1 9999 // Port number of the Raspberry Pi 2 for the first server
#define FIFO_NAME "/tmp/my_fifo"

extern int server_fd1, new_socket1; // Socket file descriptor
extern struct sockaddr_in address1; // Struct for the server address
extern int addrlen1;
extern int opt1;
extern char recvBuffer1[512]; // Buffer for the message
extern char sendBuffer1[512]; // Buffer for sending messages

void createFifo();
void init1();
void zenden1(char bericht[]);

#endif
