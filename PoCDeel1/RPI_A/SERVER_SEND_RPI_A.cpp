#include "RPI_A_Send.h"

int server_fd1, new_socket1;
struct sockaddr_in address1;
int addrlen1 = sizeof(address1);
int opt1 = 1;
char recvBuffer1[512];
char sendBuffer1[512];




int main() {
    init1(); // Initialize the server
    
    // FIFO initialisatie
    unlink("/tmp/test_fifo");
    if (mkfifo("/tmp/test_fifo", 0666) == -1) {
        perror("mkfifo failed");
        return 1;
    }

    // Open FIFO in non-blocking mode
    int fifo_fd = open("/tmp/test_fifo", O_RDONLY | O_NONBLOCK);
    if (fifo_fd == -1) {
        perror("FIFO open failed");
        return 1;
    }

    while (true) {
        // Accept socket connections
        new_socket1 = accept(server_fd1, (struct sockaddr *)&address1, (socklen_t*)&addrlen1);
        if (new_socket1 < 0) {
            perror("accept");
            continue;
        }

        fd_set readfds;
        int max_fd = (new_socket1 > fifo_fd) ? new_socket1 : fifo_fd;

        while (true) {
            FD_ZERO(&readfds);
            FD_SET(new_socket1, &readfds);
            FD_SET(fifo_fd, &readfds);

            struct timeval tv = {1, 0}; // 1 second timeout
            int retval = select(max_fd + 1, &readfds, NULL, NULL, &tv);

            if (retval == -1) {
                perror("select");
                break;
            }

            // Socket data verwerken
            if (FD_ISSET(new_socket1, &readfds)) {
                memset(recvBuffer1, 0, sizeof(recvBuffer1));
                int valread = read(new_socket1, recvBuffer1, sizeof(recvBuffer1));
                if (valread <= 0) {
                    close(new_socket1);
                    break;
                }
                std::cout << "Socket: " << recvBuffer1 << std::endl;
            }

            // FIFO data verwerken
            if (FD_ISSET(fifo_fd, &readfds)) {
                char buf[512];
                ssize_t n = read(fifo_fd, buf, sizeof(buf)-1);
                if (n > 0) {
                    buf[n] = '\0';
                    std::cout << "FIFO: " << buf;
                    zenden1(buf); // Stuur door naar socket
                }
            }
        }
    }

    close(fifo_fd);
    return 0;
}

// Rest of your functions remain the same...

void zenden1(char bericht[]) {
    if (bericht[0] != '\0'){
    send(new_socket1, bericht, strlen(bericht), 0);
    std::cout << "Sent to Server 1: " << bericht << std::endl;
}
}

void init1() {
    memset(sendBuffer1, 0, sizeof(sendBuffer1));
    server_fd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd1 == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd1, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt1, sizeof(opt1))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address1.sin_family = AF_INET;
    address1.sin_addr.s_addr = inet_addr(ServerAdres1);
    address1.sin_port = htons(PORT1);

    if (bind(server_fd1, (struct sockaddr *)&address1, sizeof(address1)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd1, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server 1 listening on port " << PORT1 << std::endl;
}
