#include "WemosSocket.h"

WemosSocket::WemosSocket() {
    memset(clientSockets, 0, sizeof(clientSockets));
}

void WemosSocket::run() {
    socketInit(); // functie uit moederklasse
    while (!quit) {
        acceptClient();
        handleClient();
    }
    cleanup();
}

void WemosSocket::acceptClient() {
    FD_ZERO(&socketSet);
    FD_SET(server_fd, &socketSet);
    max_sd = server_fd;

    for (int i = 0; i < MAXCLIENTS; i++) {
        if (clientSockets[i] > 0)
            FD_SET(clientSockets[i], &socketSet);
        if (clientSockets[i] > max_sd)
            max_sd = clientSockets[i];
    }

    if (select(max_sd + 1, &socketSet, NULL, NULL, NULL) < 0) {
        perror("select error");
        return;
    }

    if (FD_ISSET(server_fd, &socketSet)) {
        if ((sd = accept(server_fd, NULL, NULL)) < 0) {
            std::cout << "Not able to get a new client socket" << std::endl;
            return;
        }

        if (curSocket < MAXCLIENTS) {
            for (int i = 0; i < MAXCLIENTS; i++) {
                if (clientSockets[i] == 0) {
                    clientSockets[i] = sd;
                    curSocket++;
                    break;
                }
            }
        }
        else {
            std::cout << "Server is vol, client geweigerd" << std::endl;
            close(sd);
        }
    }
}

void WemosSocket::handleClient() {
    for (int i = 0; i < MAXCLIENTS; i++) {
        sd = clientSockets[i];

        if (sd > 0 && FD_ISSET(sd, &socketSet)) {
            memset(buffer, 0, sizeof(buffer));
            int valread = read(sd, buffer, sizeof(buffer) - 1);

            if (valread <= 0) {
                std::cout << "Client disconnected" << std::endl;
                close(sd);
                clientSockets[i] = 0;
                curSocket--;
                continue;
            }

            buffer[valread] = '\0';

            // Parsing van ontvangen data

            if (sscanf(buffer, "%49[^:]: %d", key, &value) == 2) {
                std::cout << "Message received: " << key << ": " << value << std::endl;

                if (strcmp(key, "Druksensor") == 0) {
                    resultaat = status.welkeWaarde(value); // statuswaarde ophalen
                    std::cout << "Processed value: " << resultaat << std::endl;
                }
                else {
                    status.checkKnop(value);
                }

                snprintf(respons, sizeof respons, "%d", resultaat);
                send(sd, respons, strlen(respons), 0);

            }
            else {
                std::cout << "Invalid message format: " << buffer << std::endl;
            }
        }
    }
}

void WemosSocket::cleanup() {
    for (int i = 0; i < MAXCLIENTS; i++) {
        if (clientSockets[i] > 0) {
            send(clientSockets[i], "Tot ziens", 9, 0);
            close(clientSockets[i]);
        }
    }
    close(server_fd);
}
