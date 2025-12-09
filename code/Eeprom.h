#ifndef EEPROM_H
#define EEPROM_H

void EepromWriteByte(unsigned char address, char value);
unsigned char EepromReadByte(unsigned char address);

#endif