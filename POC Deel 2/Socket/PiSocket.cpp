#include "PiSocket.h"
/*#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>*/

using namespace std;

PiSocket::PiSocket() : fifo_fd(-1) {
    memset(recvBuffer, 0, sizeof(recvBuffer));
    memset(sendBuffer, 0, sizeof(sendBuffer));
}

PiSocket::~PiSocket() {
    if (fifo_fd >= 0) close(fifo_fd);
    if (new_socket >= 0) close(new_socket);
    if (server_fd >= 0) close(server_fd);
    unlink("/tmp/test_fifo");
}

void PiSocket::socketInit(const char* ip, int port) {
    // Init socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(address);
    cout << "PiSocket: server listening on port " << port << endl;

    // Init FIFO
   /* unlink("/tmp/test_fifo");
    if (mkfifo("/tmp/test_fifo", 0666) == -1) {
        perror("mkfifo failed");
        exit(EXIT_FAILURE);
    }

    fifo_fd = open("/tmp/test_fifo", O_RDONLY | O_NONBLOCK);
    if (fifo_fd == -1) {
        perror("FIFO open failed");
        exit(EXIT_FAILURE);
    }*/

    cout << "FIFO initialized at /tmp/test_fifo" << endl;
}

void PiSocket::handleClient() {
    while (true) {
        cout << "Waiting for a new client connection..." << endl;
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("accept failed");
            continue;
        }

        cout << "New client connected!" << endl;
        handleConnections();
    }
}

void PiSocket::handleConnections() {
    fd_set readfds;
    int max_fd;

    while (true) {
        FD_ZERO(&readfds);

        if (new_socket >= 0) FD_SET(new_socket, &readfds);
        //if (fifo_fd >= 0) FD_SET(fifo_fd, &readfds);

        //max_fd = (new_socket > fifo_fd) ? new_socket : fifo_fd;
		max_fd = new_socket;
		
        int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0) {
            perror("select error");
            break;
        }

        if (new_socket >= 0 && FD_ISSET(new_socket, &readfds)) {
            handleSocketRead();
        }

        /*if (fifo_fd >= 0 && FD_ISSET(fifo_fd, &readfds)) {
            handleFifoRead();
        }*/
    }
}

void PiSocket::handleSocketRead() {
    memset(recvBuffer, 0, sizeof(recvBuffer));
    int valread = read(new_socket, recvBuffer, sizeof(recvBuffer) - 1);

    if (valread <= 0) {
        cout << "Client disconnected" << endl;
        close(new_socket);
        new_socket = -1;
    } else {
        recvBuffer[valread] = '\0';
        cout << "Received from socket: " << recvBuffer << endl;
        sendMessageToFifo(recvBuffer);
    }
}

void PiSocket::handleFifoRead() {
    /*char buf[512];
    ssize_t n = read(fifo_fd, buf, sizeof(buf) - 1);

    if (n > 0) {
        buf[n] = '\0';
        cout << "Received from FIFO: " << buf;
        sendMessageToPi(buf);
    }*/
}

void PiSocket::sendMessageToFifo(const char* message) {
    // Hier zou normaal een aparte write naar een tweede FIFO gebeuren als het nodig is
    cout << "Sending message to FIFO not implemented (no FIFO write fd)" << endl;
}

void PiSocket::sendMessageToPi(const char* message) {
    if (new_socket >= 0 && message[0] != '\0') {
        send(new_socket, message, strlen(message), 0);
        cout << "Sent to PiSocket Client: " << message << endl;
    }
}
