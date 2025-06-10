#include "PiSocket.h"
#include <iostream>

int main() {
    PiSocket server;

    
    server.socketInit("145.52.127.184", 9999);// IP en poort van de device zelf //145.52.127.184

    server.handleClient();

    return 0;
}
