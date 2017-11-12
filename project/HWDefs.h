#pragma once
#define TOL 1
#define NUM_MOTORS 6

const int MAX_LENGTH = 1023;
const int MIN_LENGTH = 0;
// L9958 slave select pins for SPI
#define SlaveSelectPinMotor6 32
#define SlaveSelectPinMotor5 34
#define SlaveSelectPinMotor4 36
#define SlaveSelectPinMotor3 38
#define SlaveSelectPinMotor2 40
#define SlaveSelectPinMotor1 42

// L9958 Direction pins
#define DirectionPinMotor6 33
#define DirectionPinMotor5 35
#define DirectionPinMotor4 37
#define DirectionPinMotor3 39
#define DirectionPinMotor2 41
#define DirectionPinMotor1 43

// L9958 PWM pins
#define PWMPinMotor1 13
#define PWMPinMotor2 12
#define PWMPinMotor3 11
#define PWMPinMotor4 10
#define PWMPinMotor5 9
#define PWMPinMotor6 8

//PA-14P Potentiometer pins
#define PotentiometerPinMotor1 A8
#define PotentiometerPinMotor2 A9
#define PotentiometerPinMotor3 A10
#define PotentiometerPinMotor4 A11
#define PotentiometerPinMotor5 A12
#define PotentiometerPinMotor6 A13

// L9958 Enable for all 6 motors
#define ENABLE_MOTORS1 24
#define ENABLE_MOTORS2 25

// Enum defining direction in which actuators move
typedef enum MotorDirection
{
	RETRACT = 0,
	EXTEND = 1
};

const char start_char = '-';
const char sentinel_char = '+';
const char delimiter_char = ',';
