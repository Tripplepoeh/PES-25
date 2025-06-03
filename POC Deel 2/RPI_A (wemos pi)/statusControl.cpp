#include "statusControl.h"

using namespace std;
static int bewegingsTeller = 0;


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
    static char result[512];  
    result[0] = '\0';

    // Clean input
    int j = 0;
    for (int i = 0; input[i] != '\0' && j < 511; ++i) {
        if (input[i] != '\r' && input[i] != '\n') {
            cleanedInput[j++] = input[i];
        }
    }
    cleanedInput[j] = '\0';

    std::istringstream iss(cleanedInput);
    char woord[256];

    while (iss >> woord) {
        std::transform(woord, woord + strlen(woord), woord, ::tolower);

        if (strcmp(woord, "set") == 0) {
            char sensor[256];
            uint64_t waarde;
            if (iss >> sensor >> waarde) {
                std::cout << "Set commando ontvangen: sensor = " << sensor << ", waarde = " << waarde << std::endl;
                setSensorWaarden(sensor, waarde);
                snprintf(result + strlen(result), sizeof(result) - strlen(result), "Set %s naar %d\n", sensor, waarde);
            } else {
                std::cout << "Fout bij het lezen van set commando\n";
            }
        } else if (strcmp(woord, "get") == 0) {
            char actuator[256];
            if (iss >> actuator) {
                updateActuatorStates();  // Belangrijk: refresh op basis van huidige sensoren
                int waarde = getActuatorState(actuator);
                if (waarde >= 0) {
                    snprintf(result + strlen(result), sizeof(result) - strlen(result), "%s: %s\n", actuator,
                        (waarde == 0) ? "uit" : (waarde == 1) ? "aan" : "speciaal");
                } else {
                    snprintf(result + strlen(result), sizeof(result) - strlen(result), "Onbekende actuator: %s\n", actuator);
                }
            }
        }
    }

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

int statuscontrole::getActuatorState(const char* naam) {
    for (int i = 0; i < aantalActuatoren; ++i) {
        if (strcmp(naam, actuatorNames[i]) == 0) {
            return actuatorStates[i];
        }
    }
    return -1; // Onbekende actuator
}

void statuscontrole::setSensorWaarden(const char* sensor, uint64_t waarde) {
    char message[512];
    for (int i = 0; i < aantalSensoren; ++i) {
        if (strcmp(sensor, sensorNamen[i]) == 0) {
            // altijd doorsturen voor rfidsensor, ongeacht gelijkheid
            if (sensorWaarden[i] != waarde) {
                vorigeSensorWaarden[i] = sensorWaarden[i];
                sensorWaarden[i] = waarde;
                snprintf(message, sizeof(message), "set %s %llu\n", sensor, waarde);
                stuurNaarFifo(message);
            }
            break;
        }
    }
}

void statuscontrole::updateActuatorStates() {
    

    actuatorStates[ROODLAMP] = (sensorWaarden[co2sensor] > 800) ? 1 : 0;
    actuatorStates[GROENLAMP] = (sensorWaarden[grondsensor] > 500) ? 1 : 0;
    actuatorStates[DEURSERVO] = (sensorWaarden[deurknop] == 1) ? 1 : 0;

    static auto lichtkrantStartTijd = std::chrono::steady_clock::now();
    static uint8_t lichtkrantStatus = 0;

    auto nu = std::chrono::steady_clock::now();
    auto verstreken = std::chrono::duration_cast<std::chrono::seconds>(nu - lichtkrantStartTijd).count();
    if (verstreken >= 20) {
        lichtkrantStatus ^= 1;
        lichtkrantStartTijd = nu;
    }
    actuatorStates[LICHTKRANT] = 0x70 + lichtkrantStatus;

    uint64_t code = sensorWaarden[rfidsensor];
    if (code != 0) {
        if (code == 0xB6C7283364 || code == 0x9120071DAB)
            actuatorStates[DEUR] = 1;
        else
            actuatorStates[DEUR] = 0;
    }
    //actuatorStates[LEDSTRIP] = (sensorWaarden[druksensor] >= 100) ? 0 : 1;
    //actuatorStates[LEDSTRIP] = (sensorWaarden[bewegingssensor] == 0) ? 0 : 1;
    
   if (sensorWaarden[bewegingsensor] == 1) {
    bewegingsTeller = 5;
} else if (bewegingsTeller > 0) {
    bewegingsTeller--; // Aftellen zolang geen beweging
}

// Voorwaarde gebaseerd op teller i.p.v. directe sensorwaarde
if (sensorWaarden[druksensor] >= 100 && bewegingsTeller == 0 && sensorWaarden[noodknop] == 0) {
    actuatorStates[LEDSTRIP] = 0;
} else if (bewegingsTeller == 0 && sensorWaarden[noodknop] == 0) {
    actuatorStates[LEDSTRIP] = 2;
} else {
    actuatorStates[LEDSTRIP] = 1;
}
 
    actuatorStates[DEUR] = (sensorWaarden[deurknop] == 1) ? 1 : 0;

    actuatorStates[SPECIALBEHEERDISPLAY] = static_cast<uint8_t>(sensorWaarden[co2sensor] & 0xFF);
    
    sensorWaarden[rfidsensor] = 0;
    
    actuatorStates[BUZZER] = sensorWaarden[noodknop];
    actuatorStates[GEELLAMP] = sensorWaarden[noodknop];
}

