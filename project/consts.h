/*
    Header file containing software configurations, constants, and
    clarifying definitions for the Arduino.
*/
#pragma once
#include "HWDefs.h"

#define BAUD_RATE 19200

#define NUM_MOTORS 6

#define RESET_DELAY 4000  // time (ms) for actuators to reset position during initialization

#define POSITION_TOLERANCE 1  // uncertainty for which offset from desired position is acceptable

// Pin group arrays; each value corresponding to the actuator (1-6)
const uint8_t SS_PINS[NUM_MOTORS] = { SS_PIN_1, SS_PIN_2, SS_PIN_3, SS_PIN_4, SS_PIN_5, SS_PIN_6 };
const uint8_t DIR_PINS[NUM_MOTORS] = { DIR_PIN_1, DIR_PIN_2, DIR_PIN_3, DIR_PIN_4, DIR_PIN_5, DIR_PIN_6 };
const uint8_t PWM_PINS[NUM_MOTORS] = { PWM_PIN_1, PWM_PIN_2, PWM_PIN_3, PWM_PIN_4, PWM_PIN_5, PWM_PIN_6 };
const uint8_t POT_PINS[NUM_MOTORS] = { POT_PIN_1, POT_PIN_2, POT_PIN_3, POT_PIN_4, POT_PIN_5, POT_PIN_6 };

// Extreme actuator positions; found through manual calibration
// @TODO: automate calibration for the platform?
int ZERO_POS[NUM_MOTORS] = { 199, 186, 194, 194, 192, 199 };
int MAX_POS[NUM_MOTORS] = { 833, 827, 826, 829, 831, 833 };

typedef enum MotorDirection  // to clarify the direction in which actuators move
{
    RETRACT = 0,
    EXTEND = 1
};

// SPI configuration variables (https://www.arduino.cc/en/Reference/SPI)
#define BIT_ORDER LSBFIRST
#define DATA_MODE SPI_MODE1 // clock polarity = low, phase = high 
#define SEND_INTERVAL 1000  // minimum time (ms) between printing serial info
#define INPUT_TRIGGER 15  // input (bytes) required to trigger a serial event

// Serial input limiters (to parse input characters)
const char START_CHAR = '<';
const char SENTINEL_CHAR = '>';
const char DELIMITER_CHAR = ',';
