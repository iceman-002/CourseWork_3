#include "Keypad.h"
#include "Dio.h"
#include <util/delay.h>

// Keypad Layout
const char keyMap[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

void KeypadInit(void) {
    // Setup Rows as Output
    DioSetPinDirection('b', 0, 'o');
    DioSetPinDirection('b', 1, 'o');
    DioSetPinDirection('b', 2, 'o');
    DioSetPinDirection('b', 3, 'o');
    // Setup Cols as Input
    DioSetPinDirection('d', 4, 'i');
    DioSetPinDirection('d', 5, 'i');
    DioSetPinDirection('d', 7, 'i');
    
    // Enable Pull-up resistors
    DioSetPinState('d', 4, 'h');
    DioSetPinState('d', 5, 'h');
    DioSetPinState('d', 7, 'h');
}

char KeypadGetKey(void) {
    for (int row = 0; row < 4; row++) {
        // Reset rows
        DioSetPinState('b', 0, 'h');
        DioSetPinState('b', 1, 'h');
        DioSetPinState('b', 2, 'h');
        DioSetPinState('b', 3, 'h');
        
        // Activate one row
        if(row == 0) DioSetPinState('b', 0, 'l');
        if(row == 1) DioSetPinState('b', 1, 'l');
        if(row == 2) DioSetPinState('b', 2, 'l');
        if(row == 3) DioSetPinState('b', 3, 'l');
        
        _delay_us(5);
        
        // Check columns
        if (DioReadPin('d', 4) == 0) { _delay_ms(200); return keyMap[row][0]; }
        if (DioReadPin('d', 5) == 0) { _delay_ms(200); return keyMap[row][1]; }
        if (DioReadPin('d', 7) == 0) { _delay_ms(200); return keyMap[row][2]; }
    }
    return '\0'; // No key pressed
}