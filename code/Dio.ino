#include "Dio.h"
#include <avr/io.h>

void DioInit(){
  // Prepare basic pins
  DioSetPinDirection('d', 2, 'o');
  DioSetPinDirection('d', 3, 'o');
  DioSetPinDirection('d', 4, 'o');
  DioSetPinDirection('d', 5, 'o');
  DioSetPinDirection('d', 6, 'o');
  DioSetPinDirection('d', 7, 'o');
  DioSetPinDirection('b', 0, 'o');
  DioSetPinDirection('b', 1, 'o');
}

void DioSetPinDirection(unsigned char port, int pin, unsigned char direction){
  if (port == 'b' && pin <= 7 && pin >= 0){
    if(direction == 'o') DDRB |= (1<<pin);      // Output
    else if(direction == 'i') DDRB &= ~(1<<pin); // Input
  }
  if (port == 'c' && pin <= 7 && pin >= 0){
    if(direction == 'o') DDRC |= (1<<pin);
    else if(direction == 'i') DDRC &= ~(1<<pin);
  }
  if (port == 'd' && pin <= 7 && pin >= 0){
    if(direction == 'o') DDRD |= (1<<pin);
    else if(direction == 'i') DDRD &= ~(1<<pin);
  }
}

void DioSetPinState(unsigned char port, int pin, unsigned char state){
  if (port == 'b' && pin <= 7 && pin >= 0){
    if(state == 'h') PORTB |= (1<<pin);      // High
    else if(state == 'l') PORTB &= ~(1<<pin); // Low
  }
  if (port == 'd' && pin <= 7 && pin >= 0){
    if(state == 'h') PORTD |= (1<<pin);
    else if(state == 'l') PORTD &= ~(1<<pin);
  }
  if (port == 'c' && pin <= 7 && pin >= 0){
    if(state == 'h') PORTC |= (1<<pin);
    else if(state == 'l') PORTC &= ~(1<<pin);
  }
}

unsigned char DioReadPin(unsigned char port, int pin){
    unsigned char val = 0;
    // Read the Pin state
    if (port == 'b') val = (PINB & (1<<pin)) ? 1 : 0;
    if (port == 'c') val = (PINC & (1<<pin)) ? 1 : 0;
    if (port == 'd') val = (PIND & (1<<pin)) ? 1 : 0;
    return val;
}