#ifndef I2CMASTER_H
#define I2CMASTER_H

#include <vector>
#include <unordered_map>
#include "statuscontrol.h"

class I2CMaster {
private:
    std::unordered_map<int, int> slaveFds;  // mapping: adres ? file descriptor
    statuscontrole status;                  // gedeeld object

public:
    I2CMaster();
    bool init(const std::vector<int>& slaveAddresses);  // Init met alle slaves
    void pollSlaves();  // lees en verwerk per slave
    ~I2CMaster();
};

#endif //I2CMASTER_H
