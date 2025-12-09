#include "Adc.h"
#include <avr/io.h>

void AdcInit(void) {
    ADMUX = (1<<REFS0); // Reference voltage 5V
    // Enable ADC and set clock speed
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

unsigned short AdcReadChannel(unsigned char ch) {
  ch &= 0b00000111; // Safety mask
  ADMUX = (ADMUX & 0xF8)|ch; // Select channel
  ADCSRA |= (1<<ADSC); // Start converting
  while(ADCSRA & (1<<ADSC)); // Wait for result
  return (ADC);
}