#include "statusControl.h"

using namespace std;

statuscontrole::statuscontrole(){}

statuscontrole::~statuscontrole(){
	if (fifoReadFd >= 0) close(fifoReadFd);
    if (fifoWriteFd >= 0) close(fifoWriteFd);
    unlink(FIFO_READ);
    unlink(FIFO_WRITE);
}

void statuscontrole::statuscontroleInit(){
	cout<<"statuscontrol init is gestart"<<endl;
	 mkfifo(FIFO_WRITE, 0666);
    mkfifo(FIFO_READ, 0666);
	 while ((fifoWriteFd = open(FIFO_WRITE, O_WRONLY | O_NONBLOCK)) == -1) {
        usleep(100000); 
    }
    while ((fifoReadFd = open(FIFO_READ, O_RDONLY | O_NONBLOCK)) == -1) {
        usleep(100000);
    }
    int size = sizeof(sensorWaarden)/sizeof(sensorWaarden[0]);
    for(int i = 0; i < size; i++){
		sensorWaarden[i] = 0;
		vorigeSensorWaarden[i] = 0; 
		cout<<sensorWaarden[i]<<endl;
	}
	
	cout<<"statuscontrol init is klaar"<<endl;
}

char* statuscontrole::parseInput(const char* input) {
	static char cleanedInput[512];
    static char result[512];  // Buffer voor het resultaat, static zodat het blijft bestaan na de functie
    result[0] = '\0';  // Maak de buffer leeg

    // Kopieer input naar cleanedInput en verwijder \r en \n
    int j = 0;
    for (int i = 0; input[i] != '\0' && j < 511; ++i) {
        if (input[i] != '\r' && input[i] != '\n') {
            cleanedInput[j++] = input[i];
        }
    }
    cleanedInput[j] = '\0';  // Sluit de string netjes af

    std::cout << "input is: '" << cleanedInput << "'" << std::endl;

    std::istringstream iss(cleanedInput);
    char woord[256];

    // Verwerk het input commando
    while (iss >> woord) {
        std::transform(woord, woord + strlen(woord), woord, ::tolower);

        if (strcmp(woord, "set") == 0) {
    char sensor[256];
    int waarde;
    if (iss >> sensor >> waarde) {
        std::cout << "Set commando ontvangen: sensor = " << sensor << ", waarde = " << waarde << std::endl;
        setSensorWaarden(sensor, waarde);
        snprintf(result + strlen(result), sizeof(result) - strlen(result), "Set %s naar %d\n", sensor, waarde);
    } else {
        std::cout << "Fout bij het lezen van set commando (verwacht: set <sensor> <waarde>)" << std::endl;
    }
}else if (strcmp(woord, "get") == 0) {
            char sensor[256];
            if (iss >> sensor) {
                std::cout << "Get commando ontvangen: sensor = " << sensor << std::endl;
                int waarde = getWaarde(sensor);
                snprintf(result + strlen(result), sizeof(result) - strlen(result), "Huidige waarde %s: %d\n", sensor, waarde);
            }
        }
    }

    std::cout << "result is: '" << result << "'" << std::endl;
    return result;
}
int statuscontrole::getWaarde(const char* sensor){
	  for (int i = 0; i < aantalSensoren; ++i) {
        if (strcmp(sensor, sensorNamen[i]) == 0) {  // Vergelijk de sensor namen
            return sensorWaarden[i];
        }
    }
    return -1; // Onbekende sensor
}
void statuscontrole::checkFifo(){
	char buffer[512];
    ssize_t bytesRead = read(fifoReadFd, buffer, sizeof(buffer) - 1);

    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        parseInput(buffer);
    }
}
void statuscontrole::stuurNaarFifo(const char* message){
	if (fifoWriteFd >= 0 && message != nullptr) {
        write(fifoWriteFd, message, strlen(message));
    }
}
void statuscontrole::setSensorWaarden(const char* sensor, int waarde) {
	std::cout<< "setsensorwaarde is "<< sensor << std::endl;
	char message[512];
	   for (int i = 0; i < aantalSensoren; ++i) {
        if (strcmp(sensor, sensorNamen[i]) == 0) {  // Vergelijk de sensor namen
            if (sensorWaarden[i] != waarde) {
                vorigeSensorWaarden[i] = sensorWaarden[i];
                sensorWaarden[i] = waarde;
                
                snprintf(message, sizeof(message), "Sensor waarde set naar %s: %d", sensor, waarde);
                stuurNaarFifo(message);  // Stuur het bericht via FIFO
            }
			std::cout<< "setsensorwaarde respons "<< message << std::endl;

            break;
        }
    }
}
