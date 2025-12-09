#include "Uart.h"
#include <avr/io.h>

void UartInit(void) {
    UartSetBaudRate(9600);
    UCSR0B = (1 << TXEN0) | (1 << RXEN0); // Enable Tx and Rx
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit mode
}

void UartSetBaudRate(unsigned short baudRate) {
    // Math to set communication speed
    unsigned short ubbr = ( (F_CPU / 16) /  baudRate ) - 1;
    UBRR0L = (ubbr & 0xFF);
    UBRR0H = (( ubbr >> 8 ) & 0xFF);
}

void UartSendChar(unsigned char dataByte) {
    while ( ! (UCSR0A & ( 1 << UDRE0)) ); // Wait for space
    UDR0 = dataByte;
}

unsigned char UartReadData(void) {
    while ( ! (UCSR0A & ( 1 << RXC0)) ); // Wait for data
    return UDR0;
}

void UartSendString(char *dataString) {
    int i;
    for (i=0; dataString[i]; i++) {
        UartSendChar(dataString[i]);
    }
}