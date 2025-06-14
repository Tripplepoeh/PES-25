#include "wemosServer.h" 
#include "wemosStatuscontrole.h"


using namespace std;

int main() {
    wemosStatuscontrole status;
    WemosSocket server(&status); //stuur statuscontrole object door naar wemossocket object

    server.socketInit("145.52.127.184", 12345); // IP en poort van de device zelf //145.52.127.184

    cout << "Server is gestart!" << endl;

    bool running = true; 

    while (running) { //oneindige loop
        server.acceptClient();
        server.handleClient();
        
        
    }

    server.cleanup(); // Alle clients afsluiten
    cout << "Server afgesloten." << endl;

    return 0;
}
