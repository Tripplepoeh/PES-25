#include "PiSocket.h"
#include <iostream>

int main() {
    PiSocket server;

    
    server.socketInit("192.168.68.109", 9999);// IP en poort van de device zelf //145.52.127.184

    server.handleClient();

    return 0;
}
