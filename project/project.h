// Header file containing software configurations, constants, and clarifying definitions for the Arduino.
//
#pragma once

#include "HWDefs.h"

// Local library imports (see the Github page for links/references)
#include "LinkedList.h"
#include "Regexp.h"
#include "StaticThreadController.h"
#include "Thread.h"

// Platform parameters and bounds
#define NUM_MOTORS 6
#define MIN_POS 0  // maximum and minimum position values for all actuators defined by analogRead()
#define MAX_POS 1023
#define MAX_PWM 255
const uint16_t ZERO_POS[NUM_MOTORS] = { 187, 182, 179, 187, 188, 191 };  // measured position bounds by actuator
const uint16_t END_POS[NUM_MOTORS] = { 840, 836, 835, 832, 841, 838 };
const uint16_t ADJACENT_MOTORS[NUM_MOTORS][2] = {
    // Motors indices directly adjacent to each motor; e.g. motor 5 and 1 are next to motor 0, if indexed 0 to 5
    // Pairs are ordered by direction of numbering around the platform; i.e. { reverse, forward }
    { 5, 1 }, { 0, 2 }, { 1, 3 }, { 2, 4 }, { 3, 5 }, { 4, 0 }
};

// Pin group arrays; each value corresponding to the actuator (see HWDefs.h for specific pins)
const uint8_t DIR_PINS[NUM_MOTORS] = { DIR_PIN_1, DIR_PIN_2, DIR_PIN_3, DIR_PIN_4, DIR_PIN_5, DIR_PIN_6 };
const uint8_t PWM_PINS[NUM_MOTORS] = { PWM_PIN_1, PWM_PIN_2, PWM_PIN_3, PWM_PIN_4, PWM_PIN_5, PWM_PIN_6 };
const uint8_t POT_PINS[NUM_MOTORS] = { POT_PIN_1, POT_PIN_2, POT_PIN_3, POT_PIN_4, POT_PIN_5, POT_PIN_6 };

// Movement parameters
#define RESET_DELAY 4000  // at full PWM, the actuator should fully extend/retract in 4s (6" stroke, 2.00"/s)
#define POSITION_FAR_THRESHOLD 20  // uncertainty for which offset from desired position is acceptable
#define POSITION_NEAR_THRESHOLD 5
#define PWM_NEAR 50  // value for PWM when position is in tolerance (but not in position)
#define PWM_FAR 255  // default PWM value; scaling is disabled in early development
typedef enum _MotorDirection  // to clarify the direction in which actuators move
{
    RETRACT = 0,
    EXTEND = 1
} MotorDirection;

// Serial configuration parameters
#define BAUD_RATE 9600 
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
const char TARGET_PATTERN[] = "<(%d?%d?%d?%d),"  // Lua string pattern to match proper input from the input buffer
                               "(%d?%d?%d?%d),"  // matches the last fully received proper string
                               "(%d?%d?%d?%d),"  // requires host configuration to send commands with CR+LF endings
                               "(%d?%d?%d?%d),"
                               "(%d?%d?%d?%d),"
                               "(%d?%d?%d?%d)>\n*\r*$";
