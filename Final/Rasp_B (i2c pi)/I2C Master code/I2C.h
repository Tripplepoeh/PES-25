#ifndef I2CCONTROLLER_H
#define I2CCONTROLLER_H

#include <vector>
#include <unordered_map>
//#include "statuscontrol.h"

class istatuscontrole;

class I2CMaster {
private:
    std::unordered_map<int, int> slaveFds;  
    istatuscontrole* status;            

public:
    I2CMaster(istatuscontrole* st);
    bool init(const std::vector<int>& slaveAddresses);  // Init met alle slaves
    void pollSlaves();  // lees en verwerk per slave
    ~I2CMaster();
};

#endif
