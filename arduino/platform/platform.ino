// Main Arduino code for a 6-dof Stewart platform.
// Written for the Arduino Due.
//
#include "platform.h"

// Actuator variables

uint8_t pwm[NUM_MOTORS];  // current PWM for each actuator (0 to MAX_PWM)
MotorDirection dir[NUM_MOTORS];  // current direction for each actuator (EXTEND or RETRACT)

// Position variables
int16_t pos[NUM_MOTORS];  // current position (measured by analog read) of each actuator
int16_t input[NUM_MOTORS];  // intermediate input retrieved from the serial buffer
uint16_t desired_pos[NUM_MOTORS]; // desired (user-inputted and validated) position of each actuator
int16_t pos_diff;  // difference between current and desired position
uint16_t abs_pos_diff;  // absolute pos_diff for an actuator

// Time variables (for printing)
unsigned long current_time;  // current time (in millis); used to measure difference from previous_time
unsigned long previous_time;  // last recorded time (in millis); measured from execution start or last print

// Iterator/sum variables
uint8_t motor;  // used to iterate through actuators by their indexing (0 to NUM_MOTORS - 1)
uint8_t reading;  // used to iterate through analog reads (0 to NUM_READINGS - 1)
int32_t reading_sum;  // sum of multiple readings to be averaged for a final value

/*
 * Runs once at initialization; set up input and output pins and variables.
 */
void setup()
{
    // Initialize pins
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        pinMode(DIR_PINS[motor], OUTPUT);
        digitalWrite(DIR_PINS[motor], LOW);

        pinMode(PWM_PINS[motor], OUTPUT);
        analogWrite(PWM_PINS[motor], 0);

        pinMode(POT_PINS[motor], INPUT);
    }

    // Initialize actuator enable switches
    pinMode(ENABLE_MOTORS_1, OUTPUT);
    pinMode(ENABLE_MOTORS_2, OUTPUT);
    digitalWrite(ENABLE_MOTORS_1, LOW);
    digitalWrite(ENABLE_MOTORS_2, LOW);

    // For safety, set initial actuator settings and speed to 0
    moveAll(RETRACT);
    delay(RESET_DELAY);
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        desired_pos[motor] = 0;
        pwm[motor] = 0;
    }

    // Initialize serial communication
    SerialUSB.begin(BAUD_RATE);
    SerialUSB.println("Begin");
    previous_time = 0;
}

/*
 * Run tasks in an infinite loop.
 */
void loop()
{
    // Comment out one of calibrate() and normalOp()
    // calibrate();
    // normalOp();
    moveAll(RETRACT);
}

/*
 * Read and parse serial input into actuator positions.
 */
void readSerial()
{
    // Parse ints from serial; return immediately if input is invalid
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        input[motor] = SerialUSB.parseInt();

        if (input[motor] < MIN_POS || input[motor] > MAX_POS)
        {
            return;
        }
    }

    // Set the input as the desired positions
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        desired_pos[motor] = input[motor];
    }
}

/*
 * Move all actuators in a given direction at full PWM.
 */
void moveAll(MotorDirection dir)
{
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        digitalWrite(DIR_PINS[motor], dir);
        analogWrite(PWM_PINS[motor], MAX_PWM);
    }
}

/*
 * Take NUM_READINGS readings of a potentiometer pin and returns the average.
 */
int getAverageReading(uint8_t motor)
{
    reading_sum = 0;
    for (reading = 0; reading < NUM_READINGS; ++reading)
    {
        reading_sum += analogRead(POT_PINS[motor]);
    }
    return reading_sum / NUM_READINGS;
}

/*
 * Read actuator position values from serial and move the actuators to those positions.
 */
void normalOp()
{
    // Read current actuator positions, map them based on an actuator-based scaling from calibration
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        pos[motor] = map(getAverageReading(motor),
                         ZERO_POS[motor], END_POS[motor],
                         MIN_POS, MAX_POS);
    }

    // Parse new serial input if enough is in the buffer (also sets desired position if input is valid)
    if (SerialUSB.available() > INPUT_TRIGGER)
    {
        readSerial();
    }

    // Print actuator information at a given interval
    current_time = millis();
    if (current_time - previous_time > PRINT_INTERVAL)
    {
        SerialUSB.print("Desired Positions:\n");
        for (motor = 0; motor < NUM_MOTORS; ++motor)
        {
            SerialUSB.print(desired_pos[motor]);
            SerialUSB.print(" ");
        }
        SerialUSB.print("\n");

        SerialUSB.print("Current Positions:\n");
        for (motor = 0; motor < NUM_MOTORS; ++motor)
        {
            SerialUSB.print(pos[motor]);
            SerialUSB.print(" ");
        }
        SerialUSB.print("\n");

        SerialUSB.print("PWM Values:\n");
        for (motor = 0; motor < NUM_MOTORS; ++motor)
        {
            SerialUSB.print(pwm[motor]);
            SerialUSB.print(" ");
        }
        SerialUSB.print("\n");

        previous_time = current_time;
    }

    // For each actuator, set movement parameters (direction, PWM) and execute motion
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        pos_diff = pos[motor] - desired_pos[motor];
        abs_pos_diff = abs(pos_diff);

        // Stop the actuator if it is at (or close enough to) its desired position
        if (abs_pos_diff <= POSITION_NEAR_THRESHOLD)
        {
            pwm[motor] = 0;
        }
        else
        {
            // Set a slower PWM if the actuator is close to the desired position
            pwm[motor] = (abs_pos_diff <= POSITION_FAR_THRESHOLD) ? PWM_NEAR : PWM_FAR;

            // Set the direction based on over/under-extension
            dir[motor] = (pos_diff > 0) ? RETRACT : EXTEND;
        }

        digitalWrite(DIR_PINS[motor], dir[motor]);
        analogWrite(PWM_PINS[motor], pwm[motor]);
    }
}

/*
 * Calibration routine (prints average reading at the extrema for each actuator).
 */
void calibrate()
{
    int16_t max_readings[NUM_MOTORS];  // average reading for each actuator at full extension
    int16_t min_readings[NUM_MOTORS];  // average reading for each actuator at full retraction

    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        // Start with extension
        digitalWrite(DIR_PINS[motor], EXTEND);
        analogWrite(PWM_PINS[motor], MAX_PWM);
        delay(RESET_DELAY);

        // Stop the extension, get a normalized analog reading
        analogWrite(PWM_PINS[motor], 0);
        max_readings[motor] = getAverageReading(motor);

        // Finish with retraction
        digitalWrite(DIR_PINS[motor], RETRACT);
        analogWrite(PWM_PINS[motor], MAX_PWM);
        delay(RESET_DELAY);

        // Stop the retraction, get a normalized analog reading
        analogWrite(PWM_PINS[motor], 0);
        min_readings[motor] = getAverageReading(motor);

        // Print results ("[min] [max]" - one actuator per line)
        SerialUSB.print(min_readings[motor]);
        SerialUSB.print(" ");
        SerialUSB.print(max_readings[motor]);
        SerialUSB.print("\n");
    }

    SerialUSB.print("\n");  // separate output between multiple calibration routines
}
