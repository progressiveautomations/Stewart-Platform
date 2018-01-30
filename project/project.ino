#include "HWDefs.h"

// Setup variables related to actuator position
int positions[NUM_MOTORS];
MotorDirection directions[NUM_MOTORS];
int pwms[NUM_MOTORS];

// Variables related to serial input parsing
int input[NUM_MOTORS];
int desiredPositions[NUM_MOTORS];

// Setup variables related to serial out printing
int currentTime;
int previousTime;
const int SEND_INTERVAL = 1000;
const int RESET_DELAY = 4000;

// Runs once at initialization, sets up input and output pins and variables
void setup()
{
    // Initialize pins
	for (int i = 0; i < NUM_MOTORS; ++i)
	{
		pinMode(DIR_PINS[i], OUTPUT);
		digitalWrite(DIR_PINS[i], LOW);

		pinMode(PWM_PINS[i], OUTPUT);
		analogWrite(PWM_PINS[i], 0);

		pinMode(POT_PINS[i], INPUT);

		pinMode(ENABLE_MOTORS1, OUTPUT);
		pinMode(ENABLE_MOTORS2, OUTPUT);
	}

    // Enable all motors and reset to min length
    digitalWrite(ENABLE_MOTORS1, LOW);
    digitalWrite(ENABLE_MOTORS2, LOW);
    MoveAll(RETRACT);
    delay(RESET_DELAY);
    
    // Init serial with baudrate
    SerialUSB.begin(BAUDRATE);

    // Initialize desiredPositions and pwms arrays
    for (int i = 0; i < NUM_MOTORS; ++i) 
    {
        desiredPositions[i] = 0;
        pwms[i] = 0;
    }
    
    previousTime = 0;
    // while (!SerialUSB);
    SerialUSB.println("Begin");
}

// Main function; runs tasks in infinite loop
void loop()
{
	NormalOp();
}

// Reads and parses serial input to actual actuator positions.
void ReadSerial()
{
    // Parse ints from serial
    for (int i = 0; i < NUM_MOTORS; ++i)
    {
        input[i] = SerialUSB.parseInt();
    }

    // Validate data values; return immediately if invalid
    for (int i = 0; i < NUM_MOTORS; ++i)
    {
        if (input[i] > MAX_LENGTH || input[i] < MIN_LENGTH)
        {
            //SerialUSB.print("Invalid data value: %d \n", input[i]);
            return;
        }
    }
    
    // Copy values to desiredPositions array if valid
    for (int i = 0; i < NUM_MOTORS; ++i)
    {
        desiredPositions[i] = input[i];
    }
    // SerialUSB.print("Desired positions updated! \n");
    return;
}

// Reads actuator values from serial and moves them into those positions.
void NormalOp()
{
    for (int i = 0; i < NUM_MOTORS; ++i)
    {
        // Map pot reading between MIN_LENGTH and MAX_LENGTH
		positions[i] = map(analogRead(POT_PINS[i]), ZERO_POSITION[i], MAX_POSITION[i], MIN_LENGTH, MAX_LENGTH);
	}
    
    // Parse serial input if sufficient stuff in serial input buffer
    if (SerialUSB.available() > SERIAL_MIN_AVAILABLE) ReadSerial();
        
    currentTime = millis();
    if (currentTime - previousTime > SEND_INTERVAL)
    {
        // Time to send stuff
        SerialUSB.print("Desired Positions:\n");
        for (int i = 0; i< NUM_MOTORS; ++i)
        {
            SerialUSB.print(desiredPositions[i]); SerialUSB.print(" ");
        }
        SerialUSB.print("\n");
        
        SerialUSB.print("Current Positions:\n");
        for (int i = 0; i< NUM_MOTORS; ++i)
        {
            SerialUSB.print(positions[i]); SerialUSB.print(" ");
        }
        SerialUSB.print("\n");
        
        SerialUSB.print("PWM Values:\n");
        for (int i = 0; i< NUM_MOTORS; ++i)
        {
            SerialUSB.print(pwms[i]); SerialUSB.print(" ");
        }
        SerialUSB.print("\n");
        previousTime = currentTime;
    }

    for (int i = 0; i < NUM_MOTORS; ++i)
    {
        // Check actuator position compared to desired positions
        if (fabs(positions[i] - desiredPositions[i]) <= TOL_NEAR)
        {
            // Stop moving if within tol
            pwms[i] = 0;
        }
        else
        {
            // Determine direction to move actuator
            directions[i] = (positions[i] > desiredPositions[i]) ? RETRACT : EXTEND;
            
            // Set PWM
            pwms[i] = (fabs(positions[i] - desiredPositions[i]) > TOL_FAR) ? MAX_PWM: MIN_PWM;
        }
        
        // Write values
        digitalWrite(DIR_PINS[i], directions[i]);
        analogWrite(PWM_PINS[i], pwms[i]);
    }
}

// Move all actuators in given direction
void MoveAll(MotorDirection dir)
{
    for (int i = 0; i < NUM_MOTORS; ++i) 
    {
        digitalWrite(DIR_PINS[i], dir);
        analogWrite(PWM_PINS[i], MAX_PWM);
    }
}

// Takes NUM_READINGS readings of a potentiometer pin, and
// returns the average.
int normalizeAnalogRead(int motor)
{
    const int NUM_READINGS = 10;
    int sum = 0;
    for (int r = 0; r < NUM_READINGS; ++r)
    {
        sum += analogRead(POT_PINS[motor]);
    }
    return sum / NUM_READINGS; // integer division is on purpose
}

// Looped function used to calibrate
// - Extends motors to max length, takes multiple readings and averages
// - Retracts motors to min length, takes multiple readings and averages
// - Prints calibrated values to serial
void Calibrate()
{
    int maxReadings[NUM_MOTORS];
    int minReadings[NUM_MOTORS];
    
    for (int i = 0; i < NUM_MOTORS; ++i)
    {
        // Extend motor
        digitalWrite(DIR_PINS[i], EXTEND);
        analogWrite(PWM_PINS[i], 100);
        delay(RESET_DELAY);
        
        // Read value at max extension
        analogWrite(PWM_PINS[i], 0);
        maxReadings[i] = normalizeAnalogRead(i);
        
        // Retract motor
        digitalWrite(DIR_PINS[i], RETRACT);
        analogWrite(PWM_PINS[i], 100);
        delay(RESET_DELAY);
        
        // Read value at min extension
        analogWrite(PWM_PINS[i], 0);
        minReadings[i] = normalizeAnalogRead(i);
        
        SerialUSB.print("Motor: min ");
        SerialUSB.print(minReadings[i]);
        SerialUSB.print(" max ");
        SerialUSB.print(maxReadings[i]);
        SerialUSB.println("");
    }
}
