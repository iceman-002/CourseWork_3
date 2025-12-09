#include "Lcd.h"
#include "I2c.h"
#include <util/delay.h>

#define SlaveAddress 0x27 // Address of the screen

void LcdI2cWrite(unsigned char data, unsigned char rs) {
    // Split byte into two parts (Nibbles)
    unsigned char upper = (data & 0xF0);
    unsigned char lower = ((data << 4) & 0xF0);
    unsigned char backlight = 0x08;
    unsigned char rsBit = rs ? 0x01 : 0x00;

    unsigned char dataPacket = upper | backlight | rsBit;
    
    // Send first part
    I2cStart();
    I2cWrite(SlaveAddress << 1);
    I2cWrite(dataPacket | 0x04);
    _delay_us(1);
    I2cWrite(dataPacket & ~0x04);
    _delay_us(50);
    
    // Send second part
    dataPacket = lower | backlight | rsBit;
    I2cWrite(dataPacket | 0x04);
    _delay_us(1);
    I2cWrite(dataPacket & ~0x04);
    _delay_us(50);
    
    I2cStop();
}

void LcdCommand(unsigned char cmnd) {
    LcdI2cWrite(cmnd, 0); // 0 means Command
}

void LcdChar(unsigned char data) {
    LcdI2cWrite(data, 1); // 1 means Data (Text)
}

void LcdInit(void) {
    I2cInit();
    _delay_ms(50);
    
    // Setup 4-bit mode
    LcdCommand(0x02);
    LcdCommand(0x28);
    LcdCommand(0x0C);
    LcdCommand(0x06);
    LcdCommand(0x01); // Clear screen
    _delay_ms(2);
}

void LcdString(char *str) {
    int i;
    for(i=0; str[i]!=0; i++) {
        LcdChar(str[i]);
    }
}

void LcdClear(void) {
    LcdCommand(0x01);
    _delay_ms(2);
}