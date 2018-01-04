#pragma once
#define TOL 5
#define TOL2 2
#define NUM_MOTORS 6

const int MAX_LENGTH = 1023;
const int MIN_LENGTH = 0;

int ZERO_POSITION[NUM_MOTORS] = { 199, 186, 194, 194, 192, 199 };
//int MaxPosition[NUM_MOTORS] = { 1024, 822, 821, 825, 826, 829 };
int MAX_POSITION[NUM_MOTORS] = { 833, 827, 826, 829, 831, 833 };

#define BAUDRATE 74880
// L9958 Direction pins
#define DirectionPinMotor6 33
#define DirectionPinMotor5 35
#define DirectionPinMotor4 37
#define DirectionPinMotor3 39
#define DirectionPinMotor2 41
#define DirectionPinMotor1 43
const int DIR_PINS[NUM_MOTORS] = {43, 41, 39, 37, 35, 33};

// L9958 PWM pins
#define PWMPinMotor1 13
#define PWMPinMotor2 12
#define PWMPinMotor3 11
#define PWMPinMotor4 10
#define PWMPinMotor5 9
#define PWMPinMotor6 8
const int PWM_PINS[NUM_MOTORS] = {13, 12, 11, 10, 9, 8};

//PA-14P Potentiometer pins
#define PotentiometerPinMotor1 A6
#define PotentiometerPinMotor2 A7
#define PotentiometerPinMotor3 A8
#define PotentiometerPinMotor4 A9
#define PotentiometerPinMotor5 A10
#define PotentiometerPinMotor6 A11

const int POT_PINS[NUM_MOTORS] = {A6, A7, A8, A9, A10, A11};
// L9958 Enable for all 6 motors
#define ENABLE_MOTORS1 24
#define ENABLE_MOTORS2 25

// Enum defining direction in which actuators move
typedef enum _MotorDirection
{
	RETRACT = 0,
	EXTEND = 1
} MotorDirection;

const char start_char = '-';
const char sentinel_char = '+';
const char delimiter_char = ',';
