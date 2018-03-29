// Header file containing software configurations, constants, and clarifying definitions for the Arduino.
//
# pragma once

#include "pin_layout.h"

// Platform parameters
#define NUM_MOTORS 6

// Actuator value bounds
#define MIN_POS 0
#define MAX_POS 1024
#define MIN_PWM 0
#define MAX_PWM 255

// Default platform calibration settings (average analog values at extrema for each actuator)
#define OFF_THRESHOLD 500  // ignore calibration if motors aren't powered (i.e. large reading difference from default is found)
int16_t ZERO_POS[NUM_MOTORS]  = { 188, 178, 175, 179, 189, 184 };
int16_t END_POS[NUM_MOTORS]   = { 847, 830, 839, 841, 830, 839 };

// Pin group arrays; each value corresponding to the actuator (see "pin_layout.h" for specific ins)
const uint8_t DIR_PINS[NUM_MOTORS] = { DIR_PIN_1, DIR_PIN_2, DIR_PIN_3, DIR_PIN_4, DIR_PIN_5, DIR_PIN_6 };
const uint8_t PWM_PINS[NUM_MOTORS] = { PWM_PIN_1, PWM_PIN_2, PWM_PIN_3, PWM_PIN_4, PWM_PIN_5, PWM_PIN_6 };
const uint8_t POT_PINS[NUM_MOTORS] = { POT_PIN_1, POT_PIN_2, POT_PIN_3, POT_PIN_4, POT_PIN_5, POT_PIN_6 };

// Movement parameters
#define RESET_DELAY 4000      // at full PWM, the actuator should fully extend/retract by 4s (6" stroke, 2.00"/s)
typedef enum _MotorDirection  // to clarify the direction in which actuators move
{
    RETRACT = 0,
    EXTEND  = 1
} MotorDirection;

// PID feedback parameters
const uint8_t POS_THRESHOLD[NUM_MOTORS] = { 5, 5, 5, 5, 5, 5 };
const float P_COEFF[NUM_MOTORS]         = { 3, 3, 3, 3, 3, 3 };
const float I_COEFF[NUM_MOTORS]         = { 0.035, 0.04, 0.035, 0.035, 0.03, 0.03 };
const float D_COEFF[NUM_MOTORS]         = { 0.025, 0.02, 0.025, 0.025, 0.02, 0.025 };

// Serial configuration parameters
#define BAUD_RATE 115200  // baud rate for serial port (also needs to be set on host side)

// Serial input parameters
#define INPUT_TRIGGER 10   // at minimum, 6 numbers + 5 spaces
#define NUM_READINGS  255  // number of analog readings to average to acquire position

// Serial print/output parameters
// NOTE: feedback measurement settings: PRINT_INTERVAL = 10, ENABLE_PRINT_HEADERS = 0, only PRINT_CURRENT_POS
#define PRINT_INTERVAL          1000               // minimum time (ms) between printing serial info
#define ENABLE_PRINT            1                  // flag to enable printing variables (for debugging)
#define ENABLE_PRINT_HEADERS    1                  // flag to enable variable headers (when not running analysis)
#define PRINT_DESIRED_POS       ENABLE_PRINT && 1  // print desired position values when printing serial info
#define PRINT_CURRENT_POS       ENABLE_PRINT && 1  // print current position values when printing serial info
#define PRINT_PWM               ENABLE_PRINT && 1  // print PWM values when printing serial info
