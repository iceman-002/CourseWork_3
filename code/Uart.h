#ifndef UART_H
#define UART_H

#define F_CPU 16000000UL

void UartInit(void);
void UartSetBaudRate(unsigned short baudRate);
void UartSendChar(unsigned char dataByte);
unsigned char UartReadData(void);
void UartSendString(char *dataString);

#endif