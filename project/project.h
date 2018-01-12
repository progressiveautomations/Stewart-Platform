// Header file containing software configurations, constants, and clarifying definitions for the Arduino.
//
#pragma once

// Include Arduino/PA-14P hard-defined settings
#include "HWDefs.h"

// Local library imports (see the Github page for links/references)
#include "LinkedList.h"
#include "Regexp.h"
#include "StaticThreadController.h"
#include "Thread.h"

#define NUM_MOTORS 6  // platform limitation for number of actuators

// Platform calibration settings (average analog values at extrema for each actuator)
const uint16_t ZERO_POS[NUM_MOTORS] = { 187, 182, 179, 187, 188, 191 };
const uint16_t END_POS[NUM_MOTORS] = { 840, 836, 835, 832, 841, 838 };

// Pin group arrays; each value corresponding to the actuator (see HWDefs.h for specific pins)
const uint8_t DIR_PINS[NUM_MOTORS] = { DIR_PIN_1, DIR_PIN_2, DIR_PIN_3, DIR_PIN_4, DIR_PIN_5, DIR_PIN_6 };
const uint8_t PWM_PINS[NUM_MOTORS] = { PWM_PIN_1, PWM_PIN_2, PWM_PIN_3, PWM_PIN_4, PWM_PIN_5, PWM_PIN_6 };
const uint8_t POT_PINS[NUM_MOTORS] = { POT_PIN_1, POT_PIN_2, POT_PIN_3, POT_PIN_4, POT_PIN_5, POT_PIN_6 };

// Movement parameters
#define RESET_DELAY 4000  // at full PWM, the actuator should fully extend/retract by 4s (6" stroke, 2.00"/s)
#define POSITION_FAR_THRESHOLD 20  // position uncertainty for which PWM speed decreases (to reduce overshoot)
#define POSITION_NEAR_THRESHOLD 5  // position uncertainty that is deemed acceptable (to reduce steady state jitter)
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
#define NUM_READINGS 100 // number of analog readings to average to acquire position
#define INPUT_INTERVAL 200  // interval (ms) for input thread
#define PARSER_INTERVAL 100  // interval (ms) for parser thread
#define TRANSLATOR_INTERVAL 150 // interval (ms) for translation thread
const char TARGET_PATTERN[] = "<(%d?%d?%d?%d),"  // Lua string pattern to match proper input from the input buffer
                               "(%d?%d?%d?%d),"  // matches the last fully received proper string
                               "(%d?%d?%d?%d),"  // requires host configuration to send commands with CR+LF endings
                               "(%d?%d?%d?%d),"
                               "(%d?%d?%d?%d),"
                               "(%d?%d?%d?%d)>\n*\r*$";
