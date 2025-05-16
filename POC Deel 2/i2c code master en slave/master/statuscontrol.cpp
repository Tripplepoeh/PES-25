#include "statuscontrol.h"
#include <fcntl.h>
#include <unistd.h>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <cctype>
#include <sys/stat.h>
#include <iostream>

statuscontrole::statuscontrole() {
    // Initialiseer sensors in volgorde van gebruik
    sensorIds   = {DEURKNOP, NOODKNOP, BUZZERKNOP, DRUKSENSOR,
                   GRONDSENSOR, TEMPSENSOR, RFIDSENSOR, CO2SENSOR, BEWEGINGSENSOR};
    // Initialiseer de sensorarray in dezelfde volgorde als sensorids
    sensorNames = {"deurknop","noodknop","buzzerknop","druksensor",
                   "grondsensor","tempsensor","rfidsensor","co2sensor","bewegingsensor"};
    //Geef alle sensorwaardes en oude sensorwaardes een waarde van 0
    sensorWaarden.assign(sensorIds.size(),0);
    vorigeSensorWaarden = sensorWaarden;

    // Geef aan alle actuatoren de status 0 ofwel GA_UIT
    actuatorStates.assign(256, GA_UIT);
    
    //maak een fifo pipes aan voor zowel schrijven als lezen en wacht tot de andere kant ook open is
    mkfifo(FIFO_WRITE,0666);
    mkfifo(FIFO_READ,0666);
    while((fifoWriteFd=open(FIFO_WRITE,O_WRONLY|O_NONBLOCK))<0) usleep(100000);
    while((fifoReadFd =open(FIFO_READ, O_RDONLY|O_NONBLOCK))<0) usleep(100000);
    // Registreer starttijd voor lichtkrant toggle
    lichtkrantStartTijd = std::chrono::steady_clock::now();
}

const std::vector<uint8_t>& statuscontrole::getResponses() const { return responseBuffer; } //Functie die de responsbuffer terug retourneert om naar de slave te sturen
void statuscontrole::clearResponses() { responseBuffer.clear(); } //Functie die de responsbuffer leegt om een nieuwe respons in te zetten

void statuscontrole::processI2CData(const uint8_t* d, size_t n) {
    clearResponses(); //Leeg de buffer eerst voordat nieuwe waardes worden opgeslagen en een nieuwe respons gegenereert wordt
    size_t i = 0;
    while (i < n && d[i] != GET_WAARDE && d[i] != SET_ACTUATOR) //Verwerk eerst alle sensor updates totdat de data leeg is of er een commando is
        processSensorUpdates(d, i, n);

    setActuators(); //Verwerk actuatoren met de nieuwe sensorwaardes

    while (i < n)
        processI2CCommands(d, i, n); //Verwerke alle get en set commandos
}

void statuscontrole::processSensorUpdates(const uint8_t* d, size_t& i, size_t n) {
    uint8_t id = d[i++];  // Lees sensorid van de data
    //Als het rfidsensor is, levert het 5 bytes anders 2 bytes
    size_t len = (id == RFIDSENSOR ? 5 : 2);
    if (i + len > n) return;

    // gebruik 64-bit zodat een 40-bit rfid tag/code niet afgekapt wordt
    uint64_t val = 0;
    //Zet de data om van little-endian 
    for (size_t b = 0; b < len; ++b) {
        val |= uint64_t(d[i + b]) << (8 * b);
    }
    i += len;

    // Zoek de juiste sensor en update zijn vorige en huidige waarde
    auto it = std::find(sensorIds.begin(), sensorIds.end(), id);
    if (it != sensorIds.end()) {
        size_t idx = it - sensorIds.begin();
        vorigeSensorWaarden[idx] = sensorWaarden[idx];
        sensorWaarden[idx]       = val;
    }
}


void statuscontrole::processI2CCommands(const uint8_t* d,size_t& i,size_t n) { //Verwerk commandos van de slave
    if(d[i]==GET_WAARDE && ++i<n) { //Check of de waarde een GET_WAARDE is die een actuator waarde wil krijgen
        uint8_t aid = d[i++]; //Haal de actuator id uit de data
        if(aid==SPECIALBEHEERDISPLAY) { //Als de actuator het speciaalbeheerdisplay is geet de co2sensor terug als respons
            size_t idx = std::find(sensorIds.begin(),sensorIds.end(),CO2SENSOR) - sensorIds.begin(); 
            uint16_t v = uint16_t(sensorWaarden[idx]); //casting naar 16 bytes
            responseBuffer.insert(responseBuffer.end(),{aid,uint8_t(v),uint8_t(v>>8)}); //Voeg de bytes weer toe om het 
        } else {
            // Anders retourneer de waarde van de actuator id
            responseBuffer.insert(responseBuffer.end(),{aid, actuatorStates[aid]});
        }
    } else if(d[i]==SET_ACTUATOR && i+2<n) {//Als het een set actuator command is zet de actuator waarde naar de binnenkomende waarde
        actuatorStates[d[i+1]] = d[i+2]; //zet actuator naar nieuwe waarde
        i+=3; //Verhoog de i met 3 om naar de volgende commando te gaan
    } else ++i; 
}
void statuscontrole::setActuators() { //Deze functie past de actuatorstates aan op basis van de sensor waarde
    //Dit is een soort mini functie die gemakkelijk sensorwaarde geeft van sensorids
    auto get = [&](uint8_t id)-> uint64_t{ return static_cast<uint64_t> (sensorWaarden[std::find(sensorIds.begin(),sensorIds.end(),id) - sensorIds.begin()]); };
    //Als noodknop gedrukt moeten de volgende waarde op aan gaan
    if(get(NOODKNOP)) actuatorStates[GEELLAMP] = actuatorStates[BUZZER] = GA_AAN;
    actuatorStates[ROODLAMP]  = (get(CO2SENSOR)   > 800);
    actuatorStates[GROENLAMP] = (get(GRONDSENSOR) > 500);
    actuatorStates[DEURSERVO] = get(DEURKNOP);
    //Wissel elke 20 seconden van lichtkrant status om een andere menu te tonen
    auto now = std::chrono::steady_clock::now();
    if(std::chrono::duration_cast<std::chrono::seconds>(now-lichtkrantStartTijd).count()>=20) {
        lichtkrantStatus ^= 1;
        lichtkrantStartTijd = now;
    }
    actuatorStates[LICHTKRANT] = 0x70 + lichtkrantStatus;

    uint64_t  code = get(RFIDSENSOR);
    actuatorStates[LEDSTRIP] = !get(BEWEGINGSENSOR);
    actuatorStates[DEUR]     = get(DEURKNOP) || (code==0xB6C7283364 || code==0x9120071DAB); //Doe de deur open als deurknop gedrukt is of rfid sensor die codes heeft
    actuatorStates[SPECIALBEHEERDISPLAY] = uint8_t(get(CO2SENSOR) & 0xFF);
    
  
   if(actuatorStates[DEUR]){ //verzorg dat de rfidsensor geleegd worden nadat er verzekert is gestuurd naar de slave
       std::cout<<"deur is open"<<std::endl;
       teller++;
       if (teller == 5) sensorWaarden[std::find(sensorIds.begin(), sensorIds.end(), RFIDSENSOR) - sensorIds.begin()] = 0; // reset RFID
       }
   
}

void statuscontrole::handleFifoRead() { //Deze functie handelt inkomende data van de fifo pipe die socket data heeft
    char buf[512];
    ssize_t len = read(fifoReadFd, buf, sizeof(buf)-1); //lees uit de fifo pipe
    if(len<=0) return; //als fifo pipe leeg is return
    buf[len] = '\0'; //voeg null terminator toe aan het einde van de string data
    std::istringstream iss(buf); //Maak van alles in de string zijn eigen aparte woord die met iss door heen te gaan is
    std::string cmd;
     iss >> cmd; //Lees het eerste woord 
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);//Zet het woord in kleine letters
    if(cmd != "set") return; //Als het woord geen set is ga terug

    std::string name; uint64_t value;
    if(!(iss >> name >> value)) return; //Ga terug als het geen geldig input met een sensor naam een waarde
    auto it = std::find(sensorNames.begin(), sensorNames.end(), name);//Zoek de sensor id op basis van de naam 
    if(it == sensorNames.end()) return;//Ga terug als er geen sensor gevonden is
    uint8_t id = sensorIds[it - sensorNames.begin()]; //Haal de id van de sensor

    bool isRfid = (id == RFIDSENSOR); //Rfid sensor heeft 5 bytes anders 2 bytes
    size_t payloadLen = isRfid ? 5 : 2;
    std::vector<uint8_t> data(1 + payloadLen); //Zet de data in een vector
    data[0] = id; //De eerste waarde uit de vector is de id
    for(size_t b = 0; b < payloadLen; ++b)
        data[1 + b] = (value >> (8*b)) & 0xFF; //Zet de waardes van de sensor om in bytes en in little endian

    size_t idx = 0; //Index
    processSensorUpdates(data.data(), idx, data.size()); //Verwerk de sensorwaardes op basis van de data
    setActuators(); //Update de actuatoren statussen op basis van de sensorwaardes
}

void statuscontrole::sendMessageToFifo() {
    std::ostringstream oss; //Output buffer vergelijkend als snprintf maar dan voor strings
    for(size_t i=0;i<sensorIds.size();++i) {
        if(sensorWaarden[i]!=vorigeSensorWaarden[i]) { //stuur alleen door naar de socket als de huidige sensorwaarde niet gelijk is aan de vorige sensorwaardes, dus alleen updates worden gestuurd
            oss<<"set "<<sensorNames[i]<<" "<<sensorWaarden[i]<<"\n"; //Formateer de string naar een set <sensornaam> <sensorwaarde> format 
            vorigeSensorWaarden[i] = sensorWaarden[i];
        }
    }
    std::string out = oss.str(); //Converteer de oss naar een string
    if(!out.empty()) write(fifoWriteFd, out.c_str(), out.size()); //Stuur de string via fifo
}
