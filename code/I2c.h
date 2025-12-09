#ifndef I2C_H
#define I2C_H

void I2cInit(void);
void I2cStart(void);
void I2cStop(void);
void I2cWrite(unsigned char data);

#endif