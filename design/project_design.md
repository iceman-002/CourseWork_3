# Detailed Design Document

| **Author**              | `Fahd Mohamed & Omar Yousef`                         |
|:------------------------|:-----------------------------------------------------|
| **Status**              | `submitted`                                          |
| **Version**             | `N/A`                                                |
| **Date**                | `9/12/2026`                                          |

## Introduction

This project describes the design and implementation of an embedded climate control system that shows essential features of modern automotive air-conditioning units. The system combines several components, including an LCD display, keypad interface, EEPROM memory, motor driver, and I²C-based temperature sensors. It monitors environmental conditions and lets users set their preferred temperature.

To provide persistence over power cycles, the controller maintains the user-selected temperature level in non-volatile memory after reading real-time temperature data from two sensors via the I2C connection.  An LCD module shows crucial system parameters while a DC fan motor is run in both directions at different speeds to mimic airflow management.  The user can interactively change temperature settings using the keypad or incremental buttons.

This project offers a realistic depiction of a vehicle air conditioning systems by integrating sensing, user interface, control logic, and actuation.  It demonstrates how embedded systems are utilized in industrial applications to control comfort settings, react to user input, and provide dependable, real-time control.

* Identify the system/product using any applicable names and/or version numbers.

### Purpose

The goal of this project is to design and build an embedded climate control system that can monitor temperature, accept user-defined settings, and control a fan motor. The system will show how solutions based on microcontrollers can manage environmental conditions through sensing, user interaction, and actuation.

In particular, the project allows users to choose a desired temperature using a keypad, maintains these settings in EEPROM for long-term preservation, and uses system logic to change the fan's direction and speed.  The project simulates the key features of an automobile air conditioning management unit by combining an LCD for real-time parameter display and using I2C connection for sensor data collecting.

### Scope

The project involves designing, implementing, and evaluating a microcontroller-based climate control system that mimics the key functions of an automotive air-conditioning unit. The system includes temperature measurement with two sensors linked through the I²C bus. Users can interact with it via a keypad or buttons to set their preferred temperature. It also saves these settings in EEPROM for later use. Additionally, it offers complete control over a DC fan motor, allowing directional changes and variable speed operation based on the configured logic, as well as real-time display of system parameters on an LCD display. The project does not go into advanced automobile diagnostic features or refrigeration mechanics; instead, it concentrates only on embedded control and user-interface functionality.



## Architectural Overview

  @startuml
skinparam componentStyle uml2
skinparam packageStyle rectangle
skinparam linetype polyline 
skinparam nodesep 80
skinparam ranksep 80

left to right direction

skinparam node {
    BackgroundColor<<MCU>> LightSkyBlue
    BackgroundColor<<Hardware>> HoneyDew
}
skinparam component {
    BackgroundColor<<Software>> Lavender
}

title System Context & Deployment View

node "ATmega328P (MCU)" <<MCU>> as MCU {
    component "Firmware\n(Main + Drivers)" as Firmware <<Software>>
}

package "External Peripherals" <<Hardware>> {
    
    rectangle "Actuators" {
        component "DC Motor + Driver" as Motor
    }

    rectangle "User Interface" {
        component "LCD Screen (I2C)" as LCD
        component "Matrix Keypad" as Keypad
    }

    rectangle "Sensors" {
        component "LM35 Temp Sensor" as Sensor
    }

    rectangle "Debug" {
        component "PC Serial Monitor" as Serial
    }
}

Firmware --> Motor : PWM Speed & Dir\n(Port D)

Firmware --> LCD : I2C Protocol\n(SDA / SCL)

Firmware <-- Keypad : Scan Matrix\n(Port B & D)
Firmware <-- Sensor : Analog Voltage\n(ADC Pin)

Firmware <--> Serial : UART / USB\n(TX / RX)

@enduml

## Implementation of the Module

This chapter outlines the software design of the Automotive Air Conditioning Control Panel, implemented on an ATmega328P using a layered architecture for modularity.

Core Functional Modules:

Temperature Acquisition: The internal ADC reads analog data from the LM35 sensor, converting it to a digital value for system processing.

Motor Control: A bidirectional DC motor acts as the fan. The system uses PWM for speed control and GPIO pins to toggle between heating and cooling directions.

User Interface (HMI):

LCD: An I2C-based 16x2 LCD displays real-time parameters like current temp, setpoint, and mode.

Keypad: A matrix keypad driver handles user inputs for temperature configuration and mode selection.

Data Persistence: System settings are stored in an external EEPROM over I2C, ensuring configurations (like the last set temperature) are saved across power cycles.

Communication: A UART interface transmits logs to a PC for debugging and performance monitoring.


### Assumptions & Constraints
@startuml
skinparam componentStyle uml2
skinparam noteBackgroundColor #fdfcdc
skinparam noteBorderColor #f0c040
skinparam packageStyle rectangle

title Low-Level Design Constraints & Assumptions

package "1. Design Constraints" {
    
    class "Hardware Limitations" as HW_Const {
        -- Microcontroller --
        * [cite_start]MCU: ATmega328P (8-bit Architecture) [cite: 117]
        * [cite_start]Clock Speed: 16 MHz (F_CPU) [cite: 28]
        * [cite_start]ADC Resolution: 10-bit (0-1023 value range) [cite: 53]
        * [cite_start]PWM Resolution: 8-bit (0-255 duty cycle) [cite: 39]
        * EEPROM Size: 1KB Internal Memory
    }

    class "Software Limitations" as SW_Const {
        -- Architecture --
        * [cite_start]Blocking Superloop: Non-preemptive execution [cite: 7]
        * [cite_start]Timing: Relies on _delay_ms (Freezes CPU) [cite: 27]
        -- Communication --
        * [cite_start]UART Baud Rate: Fixed at 9600 bps [cite: 28]
        * [cite_start]I2C Mode: Standard polling implementation [cite: 36]
    }
}

package "2. Design Assumptions" {

    class "System Environment" as Env_Assump {
        -- Power & Electrical --
        * [cite_start]Supply Voltage: Stable +5V DC (Required for ADC ref) [cite: 51]
        * [cite_start]Motor Driver: H-Bridge accepting Logic High/Low signals [cite: 44]
        * [cite_start]Sensor Linearity: LM35 outputs linear 10mV/°C [cite: 56]
    }

    class "Operational Scope" as Op_Assump {
        -- User Interaction --
        * Single Tasking: User does not press multiple keys simultaneously
        * [cite_start]Safety: Motor duty cycle < 100 (approx 40%) will not stall permanently [cite: 43]
        * [cite_start]Reset: System variables re-read from EEPROM only on Power-On [cite: 3]
    }
}

' Visual Relationships
HW_Const -[hidden]down- SW_Const
Env_Assump -[hidden]down- Op_Assump

@enduml

## Functional Description
The climate control system continuously measures the ambient temperature using two sensors that are connected via the I2C communication bus. The microprocessor compares the temperature measurements with the user-defined setpoint, which is entered using a keypad or adjustment buttons.   The system may retain its most recent setting even after a power cycle since EEPROM stores the selected temperature.

 The system regulates a DC fan motor to adjust airflow based on the discrepancy between the observed temperature and the necessary temperature.  The fan mimics the action of an automobile air conditioning blower system by rotating in both directions and running at various speeds.  The LCD provides the user with real-time updates throughout operation by showing the setpoint, the current temperature, and any active fan modes.This setup shows how sensing, memory, user input, and actuation come together to create a complete embedded climate control solution.

## Integration and Configuration

System configuration is managed through pre-processor macros defined in header files. This approach allows for adjustments to hardware mappings, communication parameters, and memory addresses without modifying the core application logic.

The key configuration parameters are detailed below:

* **F_CPU**
    Defines the CPU clock frequency (16 MHz) in `Uart.h` to ensure accurate baud rate calculations.

* **MIN_PWM_LIMIT**
    Sets the minimum PWM duty cycle (100) in `DcMotor.ino` required to overcome motor static friction.

* **SENSOR_CHANNEL**
    Specifies the ADC channel index (1) connected to the LM35 temperature sensor in `Lm35.ino`.

* **SlaveAddress**
    Sets the I2C bus address (0x27) for the 16x2 LCD backpack in `Lcd.ino`.

* **EEPROM Addresses**
    Allocates memory slots for storing system settings:
    * *TEMP_ADDR*: Target temperature.
    * *DIR_ADDR*: Motor direction.
    * *SPEED_ADDR*: Motor speed.
    * *MODE_ADDR*: System operation mode (Auto/Manual).
### Static Files

| File name      | Contents                                                                           |
-------------------------------------------------------------------------------------------------------
| Main.ino       | The main code. It runs the loop and decides what to do.                            |
| Main.h         | A list of the modes (Auto, Manual, etc.) the system can use.                       |
| DcMotor.ino    | The code that makes the motor start, stop, and spin.                               |
| DcMotor.h      | The menu of motor tools ("Start" and "Stop") for the main code.                    |
| Lcd.ino        | The code that draws letters and numbers on the screen.                             |
| Lcd.h          | The menu of screen tools (like "Print" or "Clear") for the main code.              |
| Keypad.ino     | The code that checks if you pressed a button.                                      |
| Keypad.h       | The tool to ask "Which key was pressed?"                                           |
| Lm35.ino       | The code that reads the sensor to see how hot it is.                               |
| Lm35.h         | The tool to ask "What is the temperature right now?"                               |
| Dio.ino        | Low-level code to turn electricity on or off for specific pins.                    |
| Dio.h          | The menu of pin tools (Input, Output, High, Low).                                  |
| Adc.ino        | Code that turns real-world voltage into a number the chip understands.             |
| Adc.h          | The tool used to read those voltage numbers.                                       |
| Pwm.ino        | Code that flicks power on and off fast to control speed.                           |
| Pwm.h          | The tool to set the speed (Duty Cycle).                                            |
| I2c.ino        | Code to talk to the screen backpack using just two wires.                          |
| I2c.h          | The tools to send data over those two wires.                                       |
| Uart.ino       | Code to send text messages to your computer.                                       |
| Uart.h         | The tools to send or receive those messages.                                       |
| Eeprom.ino     | Code to save your settings so they stay even if power goes out.                    |
| Eeprom.h       | The tools to write or read those saved settings.                                   |
| macros_types.h | A helper file with shortcuts and easy names for number types.                      |


### Include Structure

@startuml
skinparam componentStyle uml2
skinparam packageStyle rectangle
skinparam linetype ortho
skinparam nodesep 50
skinparam ranksep 50

skinparam package {
    BackgroundColor<<Application>> HoneyDew
    BackgroundColor<<HAL>> Lavender
    BackgroundColor<<MCAL>> LightSkyBlue
    BackgroundColor<<Common>> LightGray
}

title Temperature Control System - Layered Architecture

package "Common Files" <<Common>> {
    file "macros_types.h"
}

package "Application Layer" <<Application>> {
    rectangle "Main System" {
        file "Main.h"
        file "Main.ino"
    }
}

package "HAL Layer" <<HAL>> {
    
    rectangle "LCD Module" {
        file "Lcd.h"
        file "Lcd.ino"
    }
    
    rectangle "Keypad Module" {
        file "Keypad.h"
        file "Keypad.ino"
    }

    rectangle "Temp Sensor Module" {
        file "Lm35.h"
        file "Lm35.ino"
    }

    rectangle "DC Motor Module" {
        file "DcMotor.h"
        file "DcMotor.ino"
    }
}

package "MCAL Layer" <<MCAL>> {
    
    rectangle "DIO Driver" {
        file "Dio.h"
        file "Dio.ino"
    }

    rectangle "ADC Driver" {
        file "Adc.h"
        file "Adc.ino"
    }
    
    rectangle "PWM Driver" {
        file "Pwm.h"
        file "Pwm.ino"
    }
    
    rectangle "I2C Driver" {
        file "I2c.h"
        file "I2c.ino"
    }
    
    rectangle "UART Driver" {
        file "Uart.h"
        file "Uart.ino"
    }
    
    rectangle "EEPROM Driver" {
        file "Eeprom.h"
        file "Eeprom.ino"
    }
}

"Main.ino" ..> "Lcd.h"
"Main.ino" ..> "Keypad.h"
"Main.ino" ..> "Lm35.h"
"Main.ino" ..> "DcMotor.h"

"Main.ino" ..> "Uart.h"
"Main.ino" ..> "Eeprom.h"

"Lcd.ino" ..> "I2c.h"
"Keypad.ino" ..> "Dio.h"
"Lm35.ino" ..> "Adc.h"
"DcMotor.ino" ..> "Pwm.h"
"DcMotor.ino" ..> "Dio.h"

"Dio.h" .up.> "macros_types.h"

@enduml

### Configuration

| Name            | Value range   | Description                                                       |
-------------------------------------------------------------------------------------------------------
| F_CPU           | 1MHz - 20MHz  | CPU frequency in Hz. Found in Uart.h (currently 16000000UL).      |
| MIN_PWM_LIMIT   | 0 - 255       | Minimum PWM value for motor start. Found in DcMotor.ino.          |
| SENSOR_CHANNEL  | 0 - 7         | ADC channel index for LM35 sensor. Found in Lm35.ino (set to 1).  |
| SlaveAddress    | 0x00 - 0x7F   | I2C address for LCD. Found in Lcd.ino (currently 0x27).           |
| TEMP_ADDR       | 0x00 - 0x3FF  | EEPROM address for target temperature storage.                    |
| DIR_ADDR        | 0x00 - 0x3FF  | EEPROM address for motor direction storage.                       |
| SPEED_ADDR      | 0x00 - 0x3FF  | EEPROM address for motor speed storage.                           |
| MODE_ADDR       | 0x00 - 0x3FF  | EEPROM address for system mode storage.                           |
