#include "project.h"

// Initialize PWM and direction value variables
int pwm[NUM_MOTORS];
MotorDirection dir[NUM_MOTORS];

// Initialize position variables (extreme values from manual calibration)
int pos[NUM_MOTORS];
int desired_pos[NUM_MOTORS];
boolean at_correct_pos[NUM_MOTORS];

// Initalize thread objects
Thread input_thread = Thread();  // to get data from the RX buffer
Thread parser_thread = Thread();  // to parse data in the RX buffer into input
Thread translator_thread = Thread();  // to translate input into actuator movement
StaticThreadController<3> controller (&input_thread, &parser_thread, &translator_thread);

// Variables for the input thread
char input_char;  // store char from the RX buffer
LinkedList<char> char_queue; // backlog of chars from the RX buffer

// Variables for the parser thread (intermediate variables for matching the input)
MatchState ms;  // object used to match Lua string patterns
String target_string;  // string to store characters from the character queue for matching
int target_len;  // length of the target string (used to generate a temp buffer)
char match_buf[MAX_BUFFER_SIZE];  // buffer to temporarily store matched values

// Variables for the translator thread (variables for finally processed input)
int input_value;  // matched value obtained from the parser
int input_array[NUM_MOTORS];  // final array of position values from the input

// Time (ms) variables for printing
unsigned long current_time;
unsigned long previous_time;

// Flags to prevent collisions between threads
boolean buffer_locked = false;
boolean input_ready = false;
boolean input_valid = true;

int motor;  // iterator variable

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
	for (motor = 0; motor < NUM_MOTORS; motor++)
	{
		desired_pos[motor] = 0;
		pwm[motor] = 0;
	}

	// @TODO: calculate actuator lengths?

	// Configure threads for serial input processing
	input_thread.setInterval(INPUT_INTERVAL);
	parser_thread.setInterval(PARSER_INTERVAL);
	translator_thread.setInterval(TRANSLATOR_INTERVAL);

	input_thread.onRun(getInput);
	parser_thread.onRun(parseInput);
	translator_thread.onRun(translateInput);
}

/*
	Execute the loop routine, contained within the threads.

	Input thread (gets input from the RX buffer) ->
	Parser thread (parses RX input into the proper input format) ->
	Translator thread (translates input to actuator movement)
*/
void loop()
{
	controller.run();
}

/*
	Thread function to read and store serial input.
*/
void getInput()
{
	while (!buffer_locked && Serial.available() > 0)
	{
		input_char = Serial.read();
		char_queue.add(input_char);
	}

	while (!buffer_locked && char_queue.size() > MAX_BUFFER_SIZE)
	{
		buffer_locked = true;
		char_queue.remove(0);
	}
	buffer_locked = false;
}

/*
	Thread function to parse from an input Queue and send to the input thread.
*/
void parseInput()
{
	if (!buffer_locked && char_queue.size() > INPUT_TRIGGER)
	{
		// Empty the queue into the string
		buffer_locked = true;
		while (char_queue.size() > 0)
		{
			target_string += char_queue.shift();
		}
		buffer_locked = false;

		// String must be in char array format in order to be parsed
		target_len = target_string.length();
		char target_buf[target_len + 1];
		target_string.toCharArray(target_buf, target_len);

		// Parse and store the input if it is valid
		ms.Target(target_buf);
		char result = ms.Match(TARGET_PATTERN, 0);
		if (result == REGEXP_MATCHED)
		{	
			input_valid = true;
			input_ready = false; // only flag true after input is parsed and valid
			for (motor = 0; motor < NUM_MOTORS; motor++)
				{
					input_value = atoi(ms.GetCapture(match_buf, motor));
			   		if (MIN_POS <= input_value <= MAX_POS)
			   		{
			   			input_array[motor] = input_value;
			   		}
			   		else
			   		{
			   			input_valid = false;
			   			Serial.println("Invalid input value detected!");
			   			break;
			   		}
			   		input_ready = input_valid;
		   }
		}
		else
		{
			// Print the failed result for debug
			Serial.print("Unable to properly parse input!");
			Serial.print(target_buf);
			Serial.println("");
		}

		target_string = "";  // reset for next iteration
	}
}

/*
	Thread function to translate processed input to actuator movement.
*/
void translateInput()
{
	// Read potentiometer positions and scale them to [0, 1023]
	for (motor = 0; motor < NUM_MOTORS; motor++)
	{
		pos[motor] = map(analogRead(POT_PINS[motor]),
						 ZERO_POS[motor], END_POS[motor],
						 MIN_POS, MAX_POS);
	}

	// If a valid input is ready for processing, set it as the next desired position
	if (input_ready)
	{
		for (motor = 0; motor < NUM_MOTORS; motor++)
		{
			desired_pos[motor] = input_array[motor];
		}
	}

	// Print position and PWM info
	current_time = millis();
	if (current_time - previous_time > PRINT_INTERVAL)
	{
		Serial.println("Desired Positions: ");
		printMotorInfo(desired_pos);

		Serial.println("Current Positions: ");
		printMotorInfo(pos);

		Serial.println("PWM Values");
		printMotorInfo(pwm);

		previous_time = current_time;
	}

	// Check actuator positions and send movement commands as needed
	for (motor = 0; motor < NUM_MOTORS; motor++)
	{
		if (abs(pos[motor] - desired_pos[motor]) <= POSITION_TOLERANCE)
		{
			at_correct_pos[motor] = true;
			pwm[motor] = 0;
		}
		else
		{
			at_correct_pos[motor] = false;
		}

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
	Print piece of info for all actuators.

	@param pins: pin array for the info to print (position, PWM, etc.)
*/
void printMotorInfo(int pins[])
{
	for (motor = 0; motor < NUM_MOTORS; motor++)
		{
			Serial.print(pins[motor]);
			Serial.print(" ");
		}
	Serial.println("");
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
		analogWrite(PWM_PINS[motor - 1], MAX_POS);
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
	analogWrite(PWM_PINS[motor - 1], MAX_POS);
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
	}
	printMotorInfo(pos);
}
