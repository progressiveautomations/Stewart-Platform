#include "HWDefs.h"
/** Notes for porting:
*   - For AnalogIn: read_u16 -> [0, 65536], read -> [0.0, 1.0]
*   - PWM write argument must be [0.0, 1.0], alternatively specify PWM period
*/

// Setup variables related to actuator position
int positions[NUM_MOTORS];
MotorDirection directions[NUM_MOTORS];
int pwms[NUM_MOTORS];

// Variables related to serial input parsing
int input[NUM_MOTORS];
int desiredPositions[NUM_MOTORS];
bool validData = true;

// Setup variables related to serial out printing
int currentTime;
int previousTime;
const int SEND_INTERVAL = 1000;
const int RESET_DELAY = 4000;

// Move all actuators in given direction
void MoveAll(MotorDirection dir)
{
    for (int i = 0; i < NUM_MOTORS; ++i) 
    {
        digitalWrite(DIR_PINS[i], dir);
        analogWrite(PWM_PINS[i], 100);
    }
}

// Reads and parses serial input to actual actuator positions.
void ReadSerial()
{
    validData = true;
    
    // Read data according to scanf format
    while (SerialUSB.available() > 0)
    {
        for (int i = 0; i < NUM_MOTORS; ++i)
        {
            input[i] = SerialUSB.parseInt();
        }
    }
    // Validate data values
    for (int i = 0; i < NUM_MOTORS; ++i)
    {
        if (input[i] > MAX_LENGTH || input[i] < MIN_LENGTH)
        {
            //SerialUSB.print("Invalid data value: %d \n", input[i]);
            validData = false;
            return;
        }
    }
    
    // Copy values to desiredPositions array if valid
    if (validData)
    {
        for (int i = 0; i < NUM_MOTORS; ++i)
        {
            desiredPositions[i] = input[i];
        }
        // SerialUSB.print("Desired positions updated! \n");
        return;
    }
}

void setup()
{
    // Initiailize pins
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
    while (!SerialUSB);
    SerialUSB.print("Begin\n");
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


// Reads actuator values from serial and moves them into those positions.
void NormalOp()
{
    for (int i = 0; i < NUM_MOTORS; ++i)
    {
        // Map pot reading between MIN_LENGTH and MAX_LENGTH
        //NOTE: read_u16() OUTPUTS BETWEEN 0 AND 65536
        // positions[i] = (POT_PINS[i].read_u16() - ZERO_POSITION[i]) * (MAX_LENGTH - MIN_LENGTH) / (MAX_POSITION[i] - ZERO_POSITION[i]) + MIN_LENGTH; 
		positions[i] = map(analogRead(POT_PINS[i]), ZERO_POSITION[i], MAX_POSITION[i], MIN_LENGTH, MAX_LENGTH);
	}
    
    // Parse serial input
    // if (SerialUSB.available() > 0) ReadSerial();
        
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
        
    }

    for (int i = 0; i < NUM_MOTORS; ++i)
    {
        // Check actuator position compared to desired positions
        if (abs(positions[i] - desiredPositions[i]) <= TOL)
        {
            // Stop moving if within tol
            pwms[i] = 0;
        }
        else
        {
            // Determine direction to move actuator
            directions[i] = (positions[i] > desiredPositions[i]) ? RETRACT : EXTEND;
            
            // Set PWM
            // pwms[i] = abs(positions[i] - desiredPositions[i]) / MAX_LENGTH;
            pwms[i] = (abs(positions[i] - desiredPositions[i]) > TOL2) ? 100 : 50;
        }
        
        // Write values
        digitalWrite(DIR_PINS[i], directions[i]);
        analogWrite(PWM_PINS[i], pwms[i]);
    }
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

// Main function; runs setup once and tasks in infinite loop
void loop()
{
	NormalOp();
}


