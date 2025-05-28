#ifndef ALLEDEFINES_H
#define ALLEDEFINES_H

// I2C commands
static constexpr uint8_t GET_WAARDE   = 0x03;
static constexpr uint8_t SET_ACTUATOR = 0x04;
static constexpr uint8_t GA_UIT       = 0x00;
static constexpr uint8_t GA_AAN       = 0x01;

// Sensor IDs
static constexpr uint8_t DEURKNOP    = 0x20;
static constexpr uint8_t NOODKNOP    = 0x21;
static constexpr uint8_t BUZZERKNOP  = 0x22;
static constexpr uint8_t DRUKSENSOR  = 0x25;
static constexpr uint8_t GRONDSENSOR = 0x26;
static constexpr uint8_t TEMPSENSOR  = 0x27;
static constexpr uint8_t RFIDSENSOR  = 0x28;
static constexpr uint8_t CO2SENSOR   = 0x29;
static constexpr uint8_t BEWEGINGSENSOR = 0x30;
static constexpr uint8_t LUCHTVSENSOR = 0x31;

// Actuator IDs
static constexpr uint8_t LEDSTRIP            = 0x40;
static constexpr uint8_t DEUR                = 0x41;
static constexpr uint8_t DEURSERVO           = 0x42;
static constexpr uint8_t BUZZER              = 0x43;
static constexpr uint8_t LICHTKRANT          = 0x44;
static constexpr uint8_t SPECIALBEHEERDISPLAY= 0x45;
static constexpr uint8_t ROODLAMP            = 0x46;
static constexpr uint8_t GROENLAMP           = 0x47;
static constexpr uint8_t GEELLAMP            = 0x48;

#endif
