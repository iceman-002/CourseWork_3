#include "Main.h"
#include "Lcd.h"
#include "DcMotor.h"
#include "Lm35.h"
#include "Keypad.h"
#include "Eeprom.h"
#include "Uart.h"
#include "I2c.h"
#include <stdlib.h>
#include <util/delay.h>

// Where settings are saved in memory
#define TEMP_ADDR  0x10
#define DIR_ADDR   0x11
#define SPEED_ADDR 0x12
#define MODE_ADDR  0x13

// Default startup values
unsigned char systemTemp = 24;
unsigned char motorSpeed = 50;
unsigned char motorDir = 0; 
SystemState currentSystemState = STATE_AUTO;

void SystemInit(void) {
    // Turn on all hardware drivers
    DcMotorInit();
    Lm35Init();
    KeypadInit();
    LcdInit(); 
    UartInit();

    // Check memory: if saved data exists, load it
    if(EepromReadByte(TEMP_ADDR) != 0xFF) systemTemp = EepromReadByte(TEMP_ADDR);
    if(EepromReadByte(SPEED_ADDR) != 0xFF) motorSpeed = EepromReadByte(SPEED_ADDR);
    if(EepromReadByte(DIR_ADDR) != 0xFF) motorDir = EepromReadByte(DIR_ADDR);
    
    unsigned char savedMode = EepromReadByte(MODE_ADDR);
    if(savedMode != 0xFF && savedMode <= STATE_MANUAL_ON) {
        currentSystemState = (SystemState)savedMode;
    }
}

int main(void) {
    SystemInit();
    
    // Show welcome message
    LcdClear();
    LcdString("System Active");
    _delay_ms(1000);
    LcdClear();
    
    while(1) {
        unsigned char actualTemp = Lm35GetTemp(); // Check heat
        unsigned char key = KeypadGetKey();       // Check buttons
        unsigned char isSpinning = 0;

        // Handle button presses
        if (key != '\0') {
            switch(key) {
                case '3': // Switch Auto/Manual
                    if (currentSystemState == STATE_AUTO) currentSystemState = STATE_MANUAL_OFF;
                    else currentSystemState = STATE_AUTO;
                    EepromWriteByte(MODE_ADDR, currentSystemState);
                    break;

                case '1': // Switch Manual ON/OFF
                    if (currentSystemState != STATE_AUTO) {
                        if (currentSystemState == STATE_MANUAL_ON) currentSystemState = STATE_MANUAL_OFF;
                        else currentSystemState = STATE_MANUAL_ON;
                        EepromWriteByte(MODE_ADDR, currentSystemState);
                    }
                    break;

                case '2': // Increase Speed
                    if(motorSpeed < 100) motorSpeed += 10;
                    EepromWriteByte(SPEED_ADDR, motorSpeed);
                    break;
                    
                case '8': // Decrease Speed
                    if(motorSpeed > 10) motorSpeed -= 10;
                    EepromWriteByte(SPEED_ADDR, motorSpeed);
                    break;

                case '6': // Increase Temp
                    systemTemp++;
                    EepromWriteByte(TEMP_ADDR, systemTemp);
                    break;
                    
                case '4': // Decrease Temp
                    if(systemTemp > 0) systemTemp--;
                    EepromWriteByte(TEMP_ADDR, systemTemp);
                    break;

                case '5': // Toggle Direction
                    LcdClear();
                    if(motorDir == 0) {
                        motorDir = 1;
                        LcdString("Heater Mode"); 
                    } else {
                        motorDir = 0;
                        LcdString("Cooling Mode");
                    }
                    EepromWriteByte(DIR_ADDR, motorDir);
                    _delay_ms(1000); // Wait 1 second
                    LcdClear();
                    break;
            }
        }

        // Control the Motor based on Mode
        switch(currentSystemState) {
            case STATE_AUTO:
                if (actualTemp > systemTemp) {
                    DcMotorStart(0, motorDir, motorSpeed);
                    isSpinning = 1;
                } else {
                    DcMotorStop(0);
                    isSpinning = 0;
                }
                break;

            case STATE_MANUAL_ON:
                DcMotorStart(0, motorDir, motorSpeed);
                isSpinning = 1;
                break;

            case STATE_MANUAL_OFF:
                DcMotorStop(0);
                isSpinning = 0;
                break;
        }

        // Update the Screen
        char buffer[16];
        LcdCommand(0x80); // Go to Line 1
        LcdString("Act:"); 
        itoa(actualTemp, buffer, 10);
        LcdString(buffer);
        LcdString(" Set:"); 
        itoa(systemTemp, buffer, 10);
        LcdString(buffer);

        LcdCommand(0xC0); // Go to Line 2
        if(currentSystemState == STATE_AUTO) LcdString("Auto ");
        else LcdString("Man  ");

        if(isSpinning) LcdString("ON  ");
        else LcdString("OFF ");

        itoa(motorSpeed, buffer, 10);
        LcdString(buffer);
        LcdString("%");
        
        _delay_ms(150); // Stop screen flicker
    }
    return 0;
}