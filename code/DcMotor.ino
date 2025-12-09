#include "DcMotor.h"
#include "Pwm.h"
#include "Dio.h"
#include <avr/io.h>

#define MIN_PWM_LIMIT 100 // Minimum power to start spinning

void DcMotorInit(void){
    // Set control pins to Output
    DioSetPinDirection('d', 2, 'o');
    DioSetPinDirection('d', 3, 'o');
    PwmInit();
}

void DcMotorStart(unsigned char motorId, unsigned char direction, unsigned char speed){
    // Set Direction
    if (direction == 0) {
        DioSetPinState('d', 2, 'h');
        DioSetPinState('d', 3, 'l');
    } else {
        DioSetPinState('d', 2, 'l');
        DioSetPinState('d', 3, 'h');
    }
    
    // Calculate Speed (Convert % to PWM value)
    unsigned char duty;
    if (speed == 0) {
        duty = 0;
    } else {
        duty = MIN_PWM_LIMIT + (((int)speed * (255 - MIN_PWM_LIMIT)) / 100);
    }
    
    SetDutyCycle(duty);
}

void DcMotorStop(unsigned char motorId){
    // Turn everything off
    DioSetPinState('d', 2, 'l');
    DioSetPinState('d', 3, 'l');
    SetDutyCycle(0);
}