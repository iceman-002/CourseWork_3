#include "Lm35.h"
#include "Adc.h"

#define SENSOR_CHANNEL 1 

void Lm35Init(void) {
    AdcInit();
}

unsigned char Lm35GetTemp(void) {
    unsigned short adcVal = AdcReadChannel(SENSOR_CHANNEL);
    // Convert voltage to degrees Celsius
    unsigned char temp = (unsigned char)(((unsigned long)adcVal * 488) / 1000);
    return temp;
}