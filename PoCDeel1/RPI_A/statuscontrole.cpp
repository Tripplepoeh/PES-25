//#include "RPI_A_Send.h"
#include "statuscontrole.h"
#include <sys/stat.h>  // Voor mkfifo
#include <errno.h>     // Voor errno

int statuscontrole::vorigeStatus = 100;
int statuscontrole::fd = -1; 

statuscontrole::statuscontrole() : huidigeStatus(0) {
    
}

int statuscontrole::welkeWaarde(int drukWaarde) {
    if (drukWaarde >= 50) {
        huidigeStatus = 1;
        strcpy(bericht, "Er wordt geslapen");
    } else {
        huidigeStatus = 0;
        strcpy(bericht, "Er wordt niet (meer) geslapen");
        
    }

    if (huidigeStatus != vorigeStatus) {
        vorigeStatus = huidigeStatus;
        doorsturen(bericht);
    }
    
    return huidigeStatus;
}

void statuscontrole::checkKnop(int drukKnop){
	if(drukKnop == 1){
		strcpy(bericht, "Knop is gedrukt");
		doorsturen(bericht);
	}
	}

void statuscontrole::doorsturen(char bericht[]) {
    // Open FIFO alleen wanneer nodig
    int fd = open("/tmp/test_fifo", O_WRONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("open failed");
	}
    
    char msg[512];
    snprintf(msg, sizeof(msg), "%s\n", bericht);
    
    if (write(fd, msg, strlen(msg)) == -1) {
        perror("FIFO write failed");
    }
    close(fd);
}
