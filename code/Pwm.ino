#include "Pwm.h"
#include <avr/io.h>

void PwmInit(){
    DDRD |= (1<<PD6); // PWM Pin is Output
	// Setup Fast PWM mode
	TCCR0A |= (1<<COM0A1) | (1<<WGM01) | (1<<WGM00);
	TCCR0B |= (1<<CS00);
	SetDutyCycle(0);
}

void SetDutyCycle(int duty){
    OCR0A = duty; // Change pulse width
}