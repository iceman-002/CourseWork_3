#include "I2c.h"
#include <avr/io.h>

void I2cInit(void) {
    TWBR = 72; // Set bus speed
    TWSR = 0x00;
    TWCR = (1<<TWEN); // Turn on I2C
}

void I2cStart(void) {
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT))); // Wait
}

void I2cStop(void) {
    TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
}

void I2cWrite(unsigned char data) {
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT))); // Wait
}