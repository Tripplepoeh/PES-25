/*
 * i2cBeheer.hpp
 *
 *  Created on: May 11, 2025
 *      Author: aashi
 */

#ifndef INC_I2CBEHEER_HPP_
#define INC_I2CBEHEER_HPP_

#include "main.h"
#include <set>
#include <cstdint>
#include <vector>

class lamp;
class Bewegingssensor;



class I2CBeheer {
public:
    I2CBeheer();

    void I2CInit(std::vector<uint8_t> Ids,lamp* geel, lamp* groen, lamp* rood, Bewegingssensor* bws);

    void ProcessReceivedData(uint8_t *data, uint16_t size);
    const uint8_t* getBericht() const;
    uint8_t getBerichtLength() const;

private:
    std::vector<uint8_t> sensorIds;
    uint8_t txBericht[32];
    uint8_t txLength;
    std::set<uint8_t> actuatorIds;
    void voerUit(uint8_t *data, uint16_t size);
    void setBerichtKlaar();
    lamp* geelLamp;
    lamp* groenLamp;
    lamp* roodLamp;
    Bewegingssensor* bewegingsensor;
};

#endif /* INC_I2CBEHEER_HPP_ */
