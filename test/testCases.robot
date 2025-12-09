*** Test Cases ***
Test Case 1: Power-on Initialization:
    [Verify that the system initialises correctly and loads the previous settings from EEPROM]
    # Arrange 
    Power off the system
    Store values in EEPROM: Set Temp=24, Speed=50, Mode=Auto

    # Act 
    Power on the system

    # Assert 
    LCD displays "System Active" briefly then "Set: 24"
    Motor speed variable is initialized to 50

Test Case 2: Read Temperature Sensor (LM35):
    [Checking that the system accurately reads and displays the sensor value via ADC]
    # Arrange 
    Connect LM35 sensor to ADC Channel 1
    Apply a known temperature of 28 degrees

    # Act 
    Allow system to run one loop cycle

    # Assert 
    LCD displays "Act: 28"

Test Case 3: Toggle System Mode (Auto/Manual):
    [Confirm that Key '3' switches the system state between Auto and Manual]
    # Arrange 
    System is currently in STATE_AUTO

    # Act 
    Press Keypad button '3'

    # Assert 
    System State changes to STATE_MANUAL_OFF
    LCD updates display from "Auto" to "Man"
    EEPROM Mode address is updated

Test Case 4: Adjust Target Temperature:
    [Confirm that Key '6' increases and Key '4' decreases the set temperature]
    # Arrange 
    Current Set Temperature is 24 degrees

    # Act 
    Press Keypad button '6' (Up) once
    Wait 200ms
    Press Keypad button '4' (Down) once

    # Assert 
    Display updates to "Set: 25" after first press
    Display returns to "Set: 24" after second press
    New values are saved to EEPROM

Test Case 5: Change Fan Direction (Heater/Cooling Mode):
    [Verify Key '5' toggles direction and displays the correct mode message]
    # Arrange 
    Current Direction is 0 (Cooling)
    
    # Act 
    Press Keypad button '5'

    # Assert 
    LCD clears and displays "Heater Mode" for 1 second
    Motor Direction variable updates to 1
    LCD returns to main screen

Test Case 6: Manual Motor Control (ON/OFF):
    [Verify Key '1' toggles the fan on and off while in Manual Mode]
    # Arrange 
    System is in STATE_MANUAL_OFF

    # Act 
    Press Keypad button '1'

    # Assert 
    System State changes to STATE_MANUAL_ON
    LCD displays "ON"
    PWM signal is generated

Test Case 7: Manual Speed Adjustment:
    [Verify Keys '2' and '8' adjust the motor speed percentage]
    # Arrange 
    Current Motor Speed is 50%
    
    # Act 
    Press Keypad button '2' (Increase)

    # Assert 
    LCD displays "60%"
    EEPROM Speed address is updated

Test Case 8: Auto Mode Logic - Activation:
    [Verify motor turns ON automatically when Actual Temp > Set Temp]
    # Arrange 
    System is in STATE_AUTO
    Set Temperature is 24 degrees
    Increase Sensor Input (Actual) to 26 degrees

    # Act 
    System loop executes logic check

    # Assert 
    LCD displays "ON"
    DC Motor starts spinning

Test Case 9: Auto Mode Logic - Deactivation:
    [Verify motor turns OFF automatically when Actual Temp <= Set Temp]
    # Arrange 
    System is in STATE_AUTO
    Set Temperature is 24 degrees
    Motor is currently spinning
    Decrease Sensor Input (Actual) to 23 degrees

    # Act 
    System loop executes logic check

    # Assert 
    LCD displays "OFF"
    DC Motor stops spinning

Test Case 10: Speed Boundary Check:
    [Verify speed cannot exceed 100% or drop below 10%]
    # Arrange 
    Current Motor Speed is 100%
    
    # Act 
    Press Keypad button '2' (Increase)

    # Assert 
    LCD still displays "100%" (Value does not increase)