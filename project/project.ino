#include <cppQueue.h>
#include <SPI.h>
#include <StaticThreadController.h>
#include <Thread.h>

#include "HWDefs.h"
#include "consts.h"

// Initialize PWM and direction value variables
int pwm[NUM_MOTORS];
MotorDirection dir[NUM_MOTORS];

// Initialize position variables (extreme values from manual calibration)
int pos[NUM_MOTORS];
int desired_pos[NUM_MOTORS];
boolean at_correct_pos[NUM_MOTORS];

// Initialize serial input variables
unsigned long current_time;
unsigned long previous_time;

// Initialize index variables
int input_index;
int motor;

/*
	Initialize pins and actuators, and set configuration values.

	@NOTE: LOW = selected/enabled
	       HIGH = deselected/disabled
*/
void setup()
{
	Serial.begin(BAUD_RATE);

	// Initialize SPI configuration 
	SPI.begin();
	SPI.setBitOrder(BIT_ORDER);
	SPI.setDataMode(DATA_MODE);
	previous_time = 0; 

	// Initialize the pins for each actuator (slave select, direction, PWM)
	for (motor = 0; motor < NUM_MOTORS; motor++)
	{
		pinMode(SS_PINS[motor], OUTPUT);
		digitalWrite(SS_PINS[motor], LOW);
		
		pinMode(DIR_PINS[motor], OUTPUT);
		
		pinMode(PWM_PINS[motor], OUTPUT);
		digitalWrite(PWM_PINS[motor], LOW);
	}

	// Initialize actuator enable switches
	pinMode(ENABLE_MOTORS_1, OUTPUT);
	pinMode(ENABLE_MOTORS_2, OUTPUT);
	digitalWrite(ENABLE_MOTORS_1, HIGH);
	digitalWrite(ENABLE_MOTORS_2, HIGH);

	// Configure each actuator motor
	for (motor = 0; motor < NUM_MOTORS; motor++)
	{
		digitalWrite(SS_PINS[motor], LOW);
		SPI.transfer(lowByte(CONFIG_WORD));
		SPI.transfer(highByte(CONFIG_WORD));
		digitalWrite(SS_PINS[motor], HIGH);
	}

	// Initialize all actuator motors
	digitalWrite(ENABLE_MOTORS_1, LOW);
	digitalWrite(ENABLE_MOTORS_2, LOW);

	// For safety, set initial actuator settings and speed to 0  
	moveAll(RETRACT);
	delay(RESET_DELAY);
	for (motor = 0; motor < NUM_MOTORS; motor++) {
		desired_pos[motor] = 0;
		pwm[motor] = 0;
	}

	// @TODO: calculate actuator lengths?
}

/*
	Execute the loop routine.
	Default behaviour is contained in the "exec" function.
*/
void loop()
{
	exec();
}

/*
	Perform the normal routine of the loop.
*/
void exec()
{
	// Read potentiometer positions and scale them to [0, 1023]
	for (motor = 0; motor < NUM_MOTORS; motor++)
	{
		pos[motor] = map(analogRead(POT_PINS[motor]),
						 ZERO_POS[motor],
						 MAX_POS[motor],
						 0, 1024);
	}

	// Trigger a serial function if enough input is received
	if (Serial.available() >= INPUT_TRIGGER)
	{
		serialEvent();
	}

	// Print position and PWM info after a given interval
	// @ TODO: modify to stall entirely for the interval?
	current_time = millis();
	if (current_time - previous_time > SEND_INTERVAL)
	{
		Serial.println("Desired Positions: ");
		for (motor = 0; motor < NUM_MOTORS; motor++) {
			Serial.print(desired_pos[motor]);
			Serial.print(" ");
		}
		Serial.println("");

		Serial.println("Current Positions: ");
		for (motor = 0; motor < NUM_MOTORS; motor++) {
			Serial.print(pos[motor]);
			Serial.print(" ");
		}
		Serial.println("");

		Serial.println("PWM Values");
		for (motor = 0; motor < NUM_MOTORS; motor++) {
			Serial.print(pwm[motor]);
			Serial.print(" ");
		}
		Serial.println("");

		previous_time = current_time;
	}

	for (motor = 0; motor < NUM_MOTORS; motor++)
	{
		// Check the actuator position
		if (abs(pos[motor] - desired_pos[motor]) <= POSITION_TOLERANCE)
		{
			at_correct_pos[motor] = true;
			pwm[motor] = 0;
		}
		else
		{
			at_correct_pos[motor] = false;
		}

		// Set the actuator to move if needed
		if (!at_correct_pos[motor])
		{
			if (pos[motor] > desired_pos[motor])  // extended too far
			{
				dir[motor] = RETRACT;
				// pwm[motor] = abs(pos[motor] - desired_pos[motor]);
				pwm[motor] = 100; // @TODO: modify the override?
			}

			else if (pos[motor] < desired_pos[motor])  // retracted too far
			{
				dir[motor] = EXTEND;
				// pwm[motor] = abs(pos[motor] - desired_pos[motor]);
				pwm[motor] = 100;
			}
		}

		digitalWrite(DIR_PINS[motor], dir[motor]);
		analogWrite(PWM_PINS[motor], pwm[motor]);
	}
}

/*
	Process data given the trigger for data in the RX buffer.
	Each packet is intended to be enclosed by angle brackets, with comma-delimited values;
	e.g. <0,20,40,60,80,100> 

	@TODO: figure out why additional serial.printlns are required for more reliable (ish) input parsing
*/
void serialEvent()
{
	
}

/*
	Moves all actuators in a given direction.

	@param dir: direction (EXTEND/RETRACT) for the actuator
*/
void moveAll(MotorDirection dir)
{
	for (motor = 0; motor < NUM_MOTORS; motor++)
	{
		digitalWrite(DIR_PINS[motor - 1], dir);
		analogWrite(PWM_PINS[motor - 1], 1023);
	}
}

/*
	Moves one actuator in a given direction.
	Prints out raw analogRead position for the call.

	@param motor: actuator motor number to move
	@param dir: direction (EXTEND/RETRACT) for the actuator
*/
void moveOne(int motor, MotorDirection dir)
{
	digitalWrite(DIR_PINS[motor - 1], dir);
	analogWrite(PWM_PINS[motor - 1], 1023);
	Serial.println(analogRead(POT_PINS[motor - 1]));
}

/*
	Calibration routine for all actuators:

	@TODO: finish implementation.
*/
void calibrateAll()
{
	// Read potentiometer positions
	for (motor = 0; motor < NUM_MOTORS; motor++)
	{
		pos[motor] = analogRead(POT_PINS[motor]);
		Serial.print(pos[motor]);
		Serial.print(" ");
	}
	Serial.println("");
}
