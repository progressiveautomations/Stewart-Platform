// Header file containing hardware configurations for the Arduino Due mapping to the linear actuators.
//
#pragma once

// Direction pins
#define DIR_PIN_1 37
#define DIR_PIN_2 39
#define DIR_PIN_3 41
#define DIR_PIN_4 31
#define DIR_PIN_5 33
#define DIR_PIN_6 35

// PWM pins
#define PWM_PIN_1 11
#define PWM_PIN_2 12
#define PWM_PIN_3 13
#define PWM_PIN_4 8
#define PWM_PIN_5 9
#define PWM_PIN_6 10

// PA-14P potentiometer pins
#define POT_PIN_1 A6
#define POT_PIN_2 A7
#define POT_PIN_3 A8
#define POT_PIN_4 A9
#define POT_PIN_5 A10
#define POT_PIN_6 A11

// L9958 Enable for all motors (grouped as such due to different H-bridge controllers)
#define ENABLE_MOTORS_1 24
#define ENABLE_MOTORS_2 25
