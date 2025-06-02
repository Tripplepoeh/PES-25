#include "crc8.h"

uint8_t sensirion_crc8(const uint8_t *data, uint16_t count) {
    uint8_t crc = 0xFF;
    for (uint16_t i = 0; i < count; i++) {
        crc ^= data[i];
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc & 0x80) crc = (crc << 1) ^ 0x31;
            else           crc <<= 1;
        }
    }
    return crc;
}
