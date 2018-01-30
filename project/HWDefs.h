#pragma once

// Min and max lengths for actuator extension
const int NUM_MOTORS = 6;
const int MAX_LENGTH = 1023;
const int MIN_LENGTH = 0;

// Tolerances for no movement and for slow movement when close to desired acutator position
const int TOL_NEAR = 5;
const int TOL_FAR = 15;

// PWM values
const int MAX_PWM = 255;
const int MIN_PWM = 50;

const int ZERO_POSITION[NUM_MOTORS] = { 187, 182, 179, 187, 188, 191 };
const int MAX_POSITION[NUM_MOTORS] = { 840, 836, 835, 832, 841, 838 };

// Serial-related constants
const int BAUDRATE = 115200;
const int SERIAL_MIN_AVAILABLE = 10; // minimum number of bytes in serial input buffer before ReadSerial() is called

// Direction pins
const int DIR_PINS[NUM_MOTORS] = {37, 39, 41, 31, 33, 35};

// PWM pins
const int PWM_PINS[NUM_MOTORS] = {11, 12, 13, 8, 9, 10};

//PA-14P Potentiometer pins
const int POT_PINS[NUM_MOTORS] = {A6, A7, A8, A9, A10, A11};

// Enable pins for the two MultiMoto boards
const int ENABLE_MOTORS1 = 24;
const int ENABLE_MOTORS2 = 25;

// Enum defining direction in which actuators move
typedef enum _MotorDirection
{
	RETRACT = 0,
	EXTEND = 1
} MotorDirection;
