#include "Eeprom.h"
#include <avr/eeprom.h>

void EepromWriteByte(unsigned char address, char value) {
     // Save data to permanent memory
     eeprom_write_byte ((uint8_t*) (uint16_t)address, value);
}

unsigned char EepromReadByte(unsigned char address) {
    // Read data from permanent memory
    return eeprom_read_byte((uint8_t*)(uint16_t)address);
}