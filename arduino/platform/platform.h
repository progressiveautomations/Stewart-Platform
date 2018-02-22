// Header file containing software configurations, constants, and clarifying definitions for the Arduino.
//
# pragma once

#include "pin_layout.h"

#define NUM_MOTORS 6

#define MIN_POS 0
#define MAX_POS 1024

#define MIN_PWM 0
#define MAX_PWM 255

// Platform calibration settings (average analog values at extrema for each actuator)
const int16_t ZERO_POS[NUM_MOTORS] = { 183, 174, 170, 175, 185, 177 };
const int16_t END_POS[NUM_MOTORS] = { 840, 832, 837, 840, 828, 841 };

// Pin group arrays; each value corresponding to the actuator (see pin_layout.h for specific pins)
const uint8_t DIR_PINS[NUM_MOTORS] = { DIR_PIN_1, DIR_PIN_2, DIR_PIN_3, DIR_PIN_4, DIR_PIN_5, DIR_PIN_6 };
const uint8_t PWM_PINS[NUM_MOTORS] = { PWM_PIN_1, PWM_PIN_2, PWM_PIN_3, PWM_PIN_4, PWM_PIN_5, PWM_PIN_6 };
const uint8_t POT_PINS[NUM_MOTORS] = { POT_PIN_1, POT_PIN_2, POT_PIN_3, POT_PIN_4, POT_PIN_5, POT_PIN_6 };

// Movement parameters
#define RESET_DELAY 4000  // at full PWM, the actuator should fully extend/retract by 4s (6" stroke, 2.00"/s)
typedef enum _MotorDirection  // to clarify the direction in which actuators move
{
    RETRACT = 0,
    EXTEND = 1
} MotorDirection;

// Feedback parameters (for PID control)
// TODO: to be fine-tuned
#define P_COEFF 0.1
#define I_COEFF 0.1
#define D_COEFF 0.1

// Serial configuration parameters
#define BAUD_RATE 115200

// Serial input parameters
#define INPUT_TRIGGER 10  // 6 numbers + 5 spaces
#define NUM_READINGS 100 // number of analog readings to average to acquire position

// Serial output parameters
#define PRINT_INTERVAL 1000  // minimum time (ms) between printing serial info
#define RESET_DELAY 4000  // time to reach full extension/retraction at max PWM