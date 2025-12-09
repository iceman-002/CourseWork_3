#ifndef DIO_H
#define DIO_H

void DioInit(void);
void DioSetPinDirection(unsigned char port, int pin, unsigned char direction);
void DioSetPinState(unsigned char port, int pin, unsigned char state);
unsigned char DioReadPin(unsigned char port, int pin);

#endif