#include "I2C.h"
#include <iostream>
#include <vector>
#include <unistd.h>  // voor usleep
#include "statuscontrol.h"
#include "logtodatabase.h"
#include "getfromdatabase.h"
#include <memory>

int main() {
    auto log = std::make_unique<logToDatabase>();
    auto get = std::make_unique<getFromDatabase>();
    
    if (auto logCasted = dynamic_cast<logToDatabase*>(log.get())) 
        logCasted->dbInit("localhost", "rasp_b", "rasp_b", "sensordata");
    if (auto getCasted = dynamic_cast<getFromDatabase*>(get.get()))
        getCasted->dbInit("localhost", "rasp_b", "rasp_b", "sensordata");

    statuscontrole str(log.get(), get.get());
    I2CMaster controller(&str);

    std::vector<int> slaveAddresses = { 0x12, 0x20, 0x26 };

    if (!controller.init(slaveAddresses)) {
        std::cerr << "I2C initialisatie mislukt!\n";
        return 1;
    }

    std::cout << "Start polling loop...\n";

    while (true) {
        controller.pollSlaves();
        usleep(10000);  // 10ms delay
    }

    return 0;
}
