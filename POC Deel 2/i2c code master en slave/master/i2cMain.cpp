#include "I2C.h"
#include <iostream>
#include <vector>
#include <unistd.h>  // voor usleep
#include "statuscontrol.h"
#include "logtodatabase.h"

int main() {
    logToDatabase log("localhost", "rasp_b", "rasp_b", "sensordata");
    statuscontrole str(&log);
    I2CMaster controller(&str);

    std::vector<int> slaveAddresses = { 0x12, 0x20, 0x28 };

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
