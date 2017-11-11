/*
    Header file containing hardware configurations for both the Arduino
    and linear actuators.
*/
#pragma once

// L9958 slave select pins for SPI
#define SS_PIN_1 42
#define SS_PIN_2 40
#define SS_PIN_3 38
#define SS_PIN_4 36
#define SS_PIN_5 34
#define SS_PIN_6 32

// L9958 Direction pins
#define DIR_PIN_1 43
#define DIR_PIN_2 41
#define DIR_PIN_3 39
#define DIR_PIN_4 37
#define DIR_PIN_5 35
#define DIR_PIN_6 33

// L9958 PWM pins
#define PWM_PIN_1 13
#define PWM_PIN_2 12
#define PWM_PIN_3 11
#define PWM_PIN_4 10
#define PWM_PIN_5 9
#define PWM_PIN_6 8

// PA-14P potentiometer pins
#define POT_PIN_1 A8
#define POT_PIN_2 A9
#define POT_PIN_3 A10
#define POT_PIN_4 A11
#define POT_PIN_5 A12
#define POT_PIN_6 A13

// L9958 Enable for all motors (grouped as such due to different H-bridge controllers)
#define ENABLE_MOTORS_1 24
#define ENABLE_MOTORS_2 25

/******* Set up L9958 chips *********
' L9958 Config Register
' Bit
'0 - RES
'1 - DR - reset
'2 - CL_1 - curr limit
'3 - CL_2 - curr_limit
'4 - RES
'5 - RES
'6 - RES
'7 - RES
'8 - VSR - voltage slew rate (1 enables slew limit, 0 disables)
'9 - ISR - current slew rate (1 enables slew limit, 0 disables)
'10 - ISR_DIS - current slew disable
'11 - OL_ON - open load enable
'12 - RES
'13 - RES
'14 - 0 - always zero
'15 - 0 - always zero
*/
const unsigned int CONFIG_WORD = 0b0000010000001100;
