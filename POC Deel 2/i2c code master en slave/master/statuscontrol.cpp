#include "statuscontrol.h"
#include <fcntl.h>
#include <unistd.h>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <cctype>
#include <sys/stat.h>

statuscontrole::statuscontrole() {
    sensorIds   = {DEURKNOP, NOODKNOP, BUZZERKNOP, DRUKSENSOR,
                   GRONDSENSOR, TEMPSENSOR, RFIDSENSOR, CO2SENSOR, BEWEGINGSENSOR};
    sensorNames = {"deurknop","noodknop","buzzerknop","druksensor",
                   "grondsensor","tempsensor","rfidsensor","co2sensor","bewegingsensor"};
    sensorWaarden.assign(sensorIds.size(),0);
    vorigeSensorWaarden = sensorWaarden;

    // Initialize actuatorStates for IDs 0-255
    actuatorStates.assign(256, GA_UIT);

    mkfifo(FIFO_WRITE,0666);
    mkfifo(FIFO_READ,0666);
    while((fifoWriteFd=open(FIFO_WRITE,O_WRONLY|O_NONBLOCK))<0) usleep(100000);
    while((fifoReadFd =open(FIFO_READ, O_RDONLY|O_NONBLOCK))<0) usleep(100000);
    lichtkrantStartTijd = std::chrono::steady_clock::now();
}

const std::vector<uint8_t>& statuscontrole::getResponses() const { return responseBuffer; }
void statuscontrole::clearResponses() { responseBuffer.clear(); }

void statuscontrole::processI2CData(const uint8_t* d, size_t n) {
    clearResponses(); size_t i=0;
    while(i<n && d[i]!=GET_WAARDE && d[i]!=SET_ACTUATOR)
        processSensorUpdates(d,i,n);
    setActuators();
    while(i<n) processI2CCommands(d,i,n);
}



void statuscontrole::processSensorUpdates(const uint8_t* d, size_t& i, size_t n) {
    uint8_t id = d[i++];
    size_t len = (id==RFIDSENSOR ? 5 : 2);
    if(i+len>n) return;
    uint32_t val=0;
    for(size_t b=0;b<len;++b) val |= uint32_t(d[i+b]) << (8*b);
    i+=len;
    auto it = std::find(sensorIds.begin(),sensorIds.end(),id);
    if(it!=sensorIds.end()){
        size_t idx = it - sensorIds.begin();
        vorigeSensorWaarden[idx] = sensorWaarden[idx];
        sensorWaarden[idx]       = val;
    }
}

void statuscontrole::processI2CCommands(const uint8_t* d,size_t& i,size_t n) {
    if(d[i]==GET_WAARDE && ++i<n) {
        uint8_t aid = d[i++];
        if(aid==SPECIALBEHEERDISPLAY) {
            size_t idx = std::find(sensorIds.begin(),sensorIds.end(),CO2SENSOR) - sensorIds.begin();
            uint16_t v = uint16_t(sensorWaarden[idx]);
            responseBuffer.insert(responseBuffer.end(),{aid,uint8_t(v),uint8_t(v>>8)});
        } else {
            // return the stored actuator state
            responseBuffer.insert(responseBuffer.end(),{aid, actuatorStates[aid]});
        }
    } else if(d[i]==SET_ACTUATOR && i+2<n) {
        actuatorStates[d[i+1]] = d[i+2];
        i+=3;
    } else ++i;
}
void statuscontrole::setActuators() {
    auto get = [&](uint8_t id){ return sensorWaarden[std::find(sensorIds.begin(),sensorIds.end(),id) - sensorIds.begin()]; };
    if(get(NOODKNOP)) actuatorStates[GEELLAMP] = actuatorStates[BUZZER] = GA_AAN;
    actuatorStates[ROODLAMP]  = (get(CO2SENSOR)   > 800);
    actuatorStates[GROENLAMP] = (get(GRONDSENSOR) > 500);
    actuatorStates[DEURSERVO] = get(DEURKNOP);

    auto now = std::chrono::steady_clock::now();
    if(std::chrono::duration_cast<std::chrono::seconds>(now-lichtkrantStartTijd).count()>=20) {
        lichtkrantStatus ^= 1;
        lichtkrantStartTijd = now;
    }
    actuatorStates[LICHTKRANT] = 0x70 + lichtkrantStatus;

    uint32_t code = get(RFIDSENSOR);
    if(code) actuatorStates[DEUR] = (code==0xB6C7283364 || code==0x9120071DAB);

    actuatorStates[LEDSTRIP] = !get(BEWEGINGSENSOR);
    actuatorStates[DEUR]     = get(DEURKNOP);
    actuatorStates[SPECIALBEHEERDISPLAY] = uint8_t(get(CO2SENSOR) & 0xFF);
}

void statuscontrole::handleFifoRead() {
    char buf[512];
    ssize_t len = read(fifoReadFd, buf, sizeof(buf)-1);
    if(len<=0) return;
    buf[len] = '\0';
    std::istringstream iss(buf);
    std::string cmd; iss>>cmd;
    std::transform(cmd.begin(),cmd.end(),cmd.begin(),::tolower);
    if(cmd=="set"){
        std::string name; int value;
        if(!(iss>>name>>value)) return;
        auto it = std::find(sensorNames.begin(), sensorNames.end(), name);
        if(it==sensorNames.end()) return;
        uint8_t id = sensorIds[it - sensorNames.begin()];
        uint8_t data[3] = {id, uint8_t(value&0xFF), uint8_t((value>>8)&0xFF)};
        size_t idx=0;
        processSensorUpdates(data, idx, 3);
        setActuators();
    }
}

void statuscontrole::sendMessageToFifo() {
    std::ostringstream oss;
    for(size_t i=0;i<sensorIds.size();++i) {
        if(sensorWaarden[i]!=vorigeSensorWaarden[i]) {
            oss<<"set "<<sensorNames[i]<<" "<<sensorWaarden[i]<<"\n";
            vorigeSensorWaarden[i] = sensorWaarden[i];
        }
    }
    std::string out = oss.str();
    if(!out.empty()) write(fifoWriteFd, out.c_str(), out.size());
}
