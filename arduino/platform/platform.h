// Header file containing software configurations, constants, and clarifying definitions for the Arduino.
//
# pragma once

#include "pin_layout.h"

// Specify platform routine
#define ENABLE_CALIBRATION 0  // set to 1 to run a calibration routine rather than normal operation

// Platform parameters
#define NUM_MOTORS 6

// Actuator value bounds
#define MIN_POS 0
#define MAX_POS 1024
#define MIN_PWM 0
#define MAX_PWM 255

// Platform calibration settings (average analog values at extrema for each actuator)
int16_t ZERO_POS[NUM_MOTORS]  = { 188, 178, 175, 179, 189, 184 };
int16_t END_POS[NUM_MOTORS]   = { 847, 830, 839, 841, 830, 839 };

// Pin group arrays; each value corresponding to the actuator (see "pin_layout.h" for specific ins)
const uint8_t DIR_PINS[NUM_MOTORS] = { DIR_PIN_1, DIR_PIN_2, DIR_PIN_3, DIR_PIN_4, DIR_PIN_5, DIR_PIN_6 };
const uint8_t PWM_PINS[NUM_MOTORS] = { PWM_PIN_1, PWM_PIN_2, PWM_PIN_3, PWM_PIN_4, PWM_PIN_5, PWM_PIN_6 };
const uint8_t POT_PINS[NUM_MOTORS] = { POT_PIN_1, POT_PIN_2, POT_PIN_3, POT_PIN_4, POT_PIN_5, POT_PIN_6 };

// Movement parameters
#define POS_THRESHOLD 2      // distance from desired for which position is acceptable (i.e. feedback tolerance)
#define RESET_DELAY 4000     // at full PWM, the actuator should fully extend/retract by 4s (6" stroke, 2.00"/s)
typedef enum _MotorDirection  // to clarify the direction in which actuators move
{
    RETRACT = 0,
    EXTEND  = 1
} MotorDirection;

// PID feedback parameters
#define P_COEFF 5
#define I_COEFF 0.002
#define D_COEFF 0.002

// Serial configuration parameters
#define BAUD_RATE 115200  // baud rate for serial port (also needs to be set on host side)

// Serial input parameters
#define INPUT_TRIGGER 10   // at minimum, 6 numbers + 5 spaces
#define NUM_READINGS  100  // number of analog readings to average to acquire position

// Serial print/output parameters
// NOTE: feedback measurement settings: PRINT_INTERVAL = 10, ENABLE_PRINT_HEADERS = 0, only current_pos
#define PRINT_INTERVAL          10                 // minimum time (ms) between printing serial info
#define ENABLE_PRINT            0                  // flag to enable printing variables (for debugging)
#define ENABLE_PRINT_HEADERS    0                  // flag to enable variable headers (when not running analysis)
#define PRINT_DESIRED_POS       ENABLE_PRINT && 0  // print desired position values when printing serial info
#define PRINT_CURRENT_POS       ENABLE_PRINT && 1  // print current position values when printing serial info
#define PRINT_PWM               ENABLE_PRINT && 0  // print PWM values when printing serial info
