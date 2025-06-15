#include "PiSocket.h"

using namespace std;

// constructor initialiseert buffers en file descriptors
PiSocket::PiSocket() : fifo_fd(-1) {
    memset(recvBuffer, 0, sizeof(recvBuffer));
    memset(sendBuffer, 0, sizeof(sendBuffer));
}

// destructor sluit sockets en verwijdert fifo bestand
PiSocket::~PiSocket() {
    if (fifo_fd >= 0) close(fifo_fd);
    if (new_socket >= 0) close(new_socket);
    if (server_fd >= 0) close(server_fd);
    unlink("/tmp/test_fifo");
}

// initialiseer socket en fifo
void PiSocket::socketInit(const char* ip, int port) {
    // maak socket aan
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // stel socket opties in
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // stel adresstructuur in
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);

    // bind socket aan adres
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // start met luisteren
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(address);
    cout << "PiSocket: server listening on port " << port << endl;

    // maak fifo bestanden aan
    mkfifo(FIFO_WRITE, 0666);
    mkfifo(FIFO_READ, 0666);

    // open fifo voor lezen
    while ((fifoReadFd = open(FIFO_READ, O_RDONLY | O_NONBLOCK)) == -1) {
        usleep(100000);
    }

    // open fifo voor schrijven
    while ((fifoWriteFd = open(FIFO_WRITE, O_WRONLY | O_NONBLOCK)) == -1) {
        usleep(100000); 
    }

    cout << "FIFO initialized" << endl;
}
// wacht op client verbindingen en start afhandeling
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

// verwerk communicatie met socket en fifo
void PiSocket::handleConnections() {
    fd_set readfds;
    int max_fd;

    while (true) {
        // reset file descriptor set
        FD_ZERO(&readfds);

        // voeg socket en fifo toe aan set
        if (new_socket >= 0) FD_SET(new_socket, &readfds);
        if (fifoReadFd >= 0) FD_SET(fifoReadFd, &readfds);

        // bepaal hoogste file descriptor
        max_fd = max(new_socket, fifoReadFd);

        // wacht op activiteit
        int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0) {
            perror("select error");
            break;
        }

        // als er data is van client
        if (new_socket >= 0 && FD_ISSET(new_socket, &readfds)) {
            handleSocketRead();
        }

        // als er data is van fifo
        if (fifoReadFd >= 0 && FD_ISSET(fifoReadFd, &readfds)) {
            handleFifoRead();
        }
    }
}

// lees data van socket en stuur naar fifo
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
// lees data van fifo en stuur naar client
void PiSocket::handleFifoRead() {
    char buf[512];
    ssize_t n = read(fifoReadFd, buf, sizeof(buf) - 1);

    if (n > 0) {
        buf[n] = '\0';
        cout << "Received from FIFO: " << buf << endl;
        sendMessageToPi(buf);
    }
}

// schrijf bericht naar fifo
void PiSocket::sendMessageToFifo(const char* message) {
    if (fifoWriteFd >= 0 && message[0] != '\0') {
        ssize_t written = write(fifoWriteFd, message, strlen(message));
        if (written == -1) {
            perror("Failed to write to FIFO");
        } else {
            cout << "Message sent to FIFO: " << message << endl;
        }
    }
}

// stuur bericht naar client via socket
void PiSocket::sendMessageToPi(const char* message) {
    if (new_socket >= 0 && message[0] != '\0') {
        send(new_socket, message, strlen(message), 0);
        cout << "Sent to PiSocket Client: " << message << endl;
    }
}
