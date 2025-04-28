#include "PiSocket.h"

PiSocket::PiSocket() : fifo_fd(-1), new_socket(-1) {
    memset(recvBuffer, 0, sizeof(recvBuffer));
    memset(sendBuffer, 0, sizeof(sendBuffer));
}

PiSocket::~PiSocket() {
    if (fifo_fd >= 0) close(fifo_fd);
    if (new_socket >= 0) close(new_socket);
    close(server_fd);
    unlink("/tmp/test_fifo");
}

void PiSocket::run() {
    init();

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("accept");
            continue;
        }

        handleConnections();
    }
}

void PiSocket::init() {
    socketInit(); // Gebruik de Socket moederklasse

    // FIFO initialisatie
    unlink("/tmp/test_fifo");
    if (mkfifo("/tmp/test_fifo", 0666) == -1) {
        perror("mkfifo failed");
        exit(EXIT_FAILURE);
    }

    fifo_fd = open("/tmp/test_fifo", O_RDONLY | O_NONBLOCK);
    if (fifo_fd == -1) {
        perror("FIFO open failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "FIFO initialized at /tmp/test_fifo" << std::endl;
}

void PiSocket::handleConnections() {
    fd_set readfds;
    int max_fd = (new_socket > fifo_fd) ? new_socket : fifo_fd;

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(new_socket, &readfds);
        FD_SET(fifo_fd, &readfds);

        struct timeval tv = { 1, 0 }; // 1 second timeout
        int retval = select(max_fd + 1, &readfds, NULL, NULL, &tv);

        if (retval == -1) {
            perror("select error");
            break;
        }

        if (FD_ISSET(new_socket, &readfds)) {
            handleSocketRead();
        }

        if (FD_ISSET(fifo_fd, &readfds)) {
            handleFifoRead();
        }
    }
}

void PiSocket::handleSocketRead() {
    memset(recvBuffer, 0, sizeof(recvBuffer));
    int valread = read(new_socket, recvBuffer, sizeof(recvBuffer) - 1);

    if (valread <= 0) {
        std::cout << "Client disconnected from socket" << std::endl;
        close(new_socket);
        new_socket = -1;
    }
    else {
        recvBuffer[valread] = '\0';
        std::cout << "Socket: " << recvBuffer << std::endl;
    }
}

void PiSocket::handleFifoRead() {
    char buf[512];
    ssize_t n = read(fifo_fd, buf, sizeof(buf) - 1);

    if (n > 0) {
        buf[n] = '\0';
        std::cout << "FIFO: " << buf;
        sendMessage(buf);
    }
}

void PiSocket::sendMessage(const char* message) {
    if (new_socket >= 0 && message[0] != '\0') {
        send(new_socket, message, strlen(message), 0);
        std::cout << "Sent to PiSocket Client: " << message << std::endl;
    }
}
