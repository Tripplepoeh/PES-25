#include "wemosServer.h"

using namespace std;

WemosSocket::WemosSocket() {
    memset(clientSockets, 0, sizeof(clientSockets));
}

void WemosSocket::socketInit(const char* ip, int port) {
    // Socket aanmaken
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Socket opties
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);

    // Binden
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Luisteren
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    cout << "Server listening on port " << port << endl;
}

void WemosSocket::acceptClient() {
    FD_ZERO(&socketSet);
    FD_SET(server_fd, &socketSet);
    max_sd = server_fd;

    for (int i = 0; i < MAXCLIENTS; i++) {
        if (clientSockets[i] > 0) {
            FD_SET(clientSockets[i], &socketSet);
        }
        if (clientSockets[i] > max_sd) {
            max_sd = clientSockets[i];
        }
    }

    if (select(max_sd + 1, &socketSet, NULL, NULL, NULL) < 0) {
        perror("select error");
        return;
    }

    if (FD_ISSET(server_fd, &socketSet)) {
        int new_client_sd = accept(server_fd, NULL, NULL);
        if (new_client_sd < 0) {
            perror("accept");
            return;
        }

        if (curSocket < MAXCLIENTS) {
            for (int i = 0; i < MAXCLIENTS; i++) {
                if (clientSockets[i] == 0) {
                    clientSockets[i] = new_client_sd;
                    curSocket++;
                    cout << "New client connected, socket fd: " << new_client_sd << endl;
                    break;
                }
            }
        } else {
            cout << "Server is full, refusing new client" << endl;
            close(new_client_sd);
        }
    }
}

void WemosSocket::handleClient() {
    for (int i = 0; i < MAXCLIENTS; i++) {
        sd = clientSockets[i];
        if (sd > 0 && FD_ISSET(sd, &socketSet)) {
            memset(recvBuffer, 0, sizeof(recvBuffer)); // Gebruik de recvBuffer uit parent class
            int valread = read(sd, recvBuffer, sizeof(recvBuffer));

            if (valread <= 0) {
                cout << "Client disconnected, closing socket fd: " << sd << endl;
                close(sd);
                clientSockets[i] = 0;
                curSocket--;
            } else {
                recvBuffer[valread] = '\0';

                cout << "Message received: " << recvBuffer << endl;

                // Direct hele bericht doorsturen
                //status.checkInput(recvBuffer);

                // Eventueel bevestiging sturen
                const char* response = "OK";
                send(sd, response, strlen(response), 0);
            }
        }
    }
}

void WemosSocket::cleanup() {
    for (int i = 0; i < MAXCLIENTS; i++) {
        if (clientSockets[i] > 0) {
            send(clientSockets[i], "Tot ziens", strlen("Tot ziens"), 0);
            close(clientSockets[i]);
            clientSockets[i] = 0;
        }
    }
    close(server_fd);
}
