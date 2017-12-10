/*
    Header file containing software configurations, constants, and
    clarifying definitions for the Arduino.
*/
#pragma once

#include <LinkedList.h>
#include <Regexp.h>
#include <SPI.h>
#include <StaticThreadController.h>
#include <Thread.h>

#include "HWDefs.h"

// Platform parameters and bounds
#define NUM_MOTORS 6
#define MIN_POS 0  // maximum and minimum position values for all actuators defined by analogRead()
#define MAX_POS 1023
#define MAX_PWM 255
int ZERO_POS[NUM_MOTORS] = { 185, 178, 176, 179, 184, 186 };  // measured position bounds by actuator
int END_POS[NUM_MOTORS] = { 837, 835, 832, 835, 835, 840 };

// Pin group arrays; each value corresponding to the actuator (see HWDefs for values)
const uint8_t DIR_PINS[NUM_MOTORS] = { DIR_PIN_1, DIR_PIN_2, DIR_PIN_3, DIR_PIN_4, DIR_PIN_5, DIR_PIN_6 };
const uint8_t PWM_PINS[NUM_MOTORS] = { PWM_PIN_1, PWM_PIN_2, PWM_PIN_3, PWM_PIN_4, PWM_PIN_5, PWM_PIN_6 };
const uint8_t POT_PINS[NUM_MOTORS] = { POT_PIN_1, POT_PIN_2, POT_PIN_3, POT_PIN_4, POT_PIN_5, POT_PIN_6 };

// Movement parameters
#define RESET_DELAY 4000  // at full PWM, the actuator should fully extend/retract in 4s (6" stroke, 2.00"/s)
#define POSITION_TOLERANCE 10  // uncertainty for which offset from desired position is acceptable
#define PWM_NEAR 20  // value for PWM when position is in tolerance (but not in position)
#define PWM_FAR 100  // default PWM value; scaling is disabled in early development
typedef enum MotorDirection  // to clarify the direction in which actuators move
{
    RETRACT = 0,
    EXTEND = 1
};

// Serial/SPI configuration parameters (https://www.arduino.cc/en/Reference/SPI)
#define BAUD_RATE 9600
// #define BIT_ORDER LSBFIRST
// #define DATA_MODE SPI_MODE1  // clock polarity = low, phase = high 
#define PRINT_INTERVAL 2000  // minimum time (ms) between printing serial info

// Serial input parameters
#define MAX_BUFFER_SIZE 31  // 4 bytes per position (6 digits), 7 for limiter characters
#define INPUT_TRIGGER 15  // 7 limiter characters + 6 digits + 2 line endings = 15 characters minimum
#define NUM_READINGS 100 // number of analog readings to normalize to acquire position
#define INPUT_INTERVAL 200  // interval (ms) for input thread
#define PARSER_INTERVAL 100  // interval (ms) for parser thread
#define TRANSLATOR_INTERVAL 150 // interval (ms) for translation thread
const char START_CHAR = '<';
const char SENTINEL_CHAR = '>';
const char DELIMITER_CHAR = ',';
const char TARGET_PATTERN[] = "<(%d?%d?%d?%d),"  // pattern to match proper input from the input buffer
                               "(%d?%d?%d?%d),"  // matches the last fully received proper string
                               "(%d?%d?%d?%d),"  // buffer overflow issues should be handled by the hardware/interface
                               "(%d?%d?%d?%d),"
                               "(%d?%d?%d?%d),"
                               "(%d?%d?%d?%d)>\n*\r*$";
