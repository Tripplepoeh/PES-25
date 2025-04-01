#include "RPI_A_Wemos_Server.h"



using namespace std;


int main() {
  
    
    socketInit();
    quit = 0;
    socketSet;
    sd, max_sd, curSocket = 0;
    clientSockets[MAXCLIENTS] = {0};

    while (!quit) {
        // Verbin ding accepteren
        acceptClient();
        handleClient();
        
    }


for (int i = 0; i < MAXCLIENTS; i++) {
    if (clientSockets[i] > 0) {
        send(clientSockets[i], "Tot ziens", 9, 0);
        close(clientSockets[i]);
        clientSockets[i] = 0;
    }
}
    close(new_socket);
    close(server_fd);
    return 0;
}

void handleClient(){
    for (int i = 0; i < MAXCLIENTS; i++) {
        sd = clientSockets[i];
        if (sd > 0 && FD_ISSET(sd, &socketSet)) {
            memset(buffer, 0, sizeof(buffer));
            int valread = read(sd, buffer, sizeof(buffer));

            // Check of de client verbroken is
            if (valread <= 0) {
                std::cout << "Client disconnected"<< std::endl;
                close(sd);
                curSocket--;
                clientSockets[i] = 0;
                return;
            }
            buffer[valread] = '\0';

            // Parsing van ontvangen data
            
            value = 0;
            if (sscanf(buffer, "%49[^:]: %d", key, &value) == 2) {  // Zorgt ervoor dat beide waarden correct ingelezen zijn
                std::cout << "Message received: " << key << ": " << value << std::endl;

                // Verwerk waarde
                if(strcmp(key, "Druksensor") == 0){
                resultaat = status.welkeWaarde(value);
                std::cout << "Processed value: " << resultaat << std::endl;
            }else{
                status.checkKnop(value);
                }

                // Stuur een bevestiging terug naar de client
                
                snprintf(respons, sizeof respons, "%d", resultaat);
                send(sd, respons, sizeof respons, 0);
            } else {
                std::cout << "Invalid message format: " << buffer << std::endl;
            }
            }
            
        }
    }


void acceptClient(){
    FD_ZERO(&socketSet);
        FD_SET(server_fd, &socketSet);
        max_sd = server_fd;
        for(int i = 0; i < MAXCLIENTS; i++){
            if(clientSockets[i] > 0 ){
                FD_SET(clientSockets[i], &socketSet);
            }
            if (clientSockets[i] > max_sd){
                max_sd = clientSockets[i];
                }
        }
        if(select(max_sd + 1, &socketSet, NULL, NULL, NULL) < 0){
         return;
         }
      
        if(FD_ISSET(server_fd, &socketSet)){
            if ((sd = accept(server_fd, NULL, NULL)) < 0){
                cout << endl << "Not able to get a new client socket";
                return;
                }
                if (curSocket < MAXCLIENTS){
                    for (int i = 0; i < MAXCLIENTS; i++){
                        if(clientSockets[i] == 0){
                            clientSockets[i]= sd;
                            curSocket++;
                            break;
                            }
                    }
                    
                }else{
                    cout << endl << "Server is vol, clients geweigerd";
                     close(sd);
                     
                    }
                }
    }

void socketInit(){
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
    address.sin_addr.s_addr = inet_addr(ServerAdres);
    address.sin_port = htons(PORT);

    // Binden
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Luisteren
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << std::endl;
}
