// Main Arduino code for a 6-dof Stewart platform.
// Written for the Arduino Due.
//
#include "platform.h"

// Actuator variables
uint8_t pwm[NUM_MOTORS];  // current PWM for each actuator
MotorDirection dir[NUM_MOTORS];  // current direction for each actuator (EXTEND or RETRACT)

// Position variables
int16_t pos[NUM_MOTORS];  // current position (measured by analog read) of each actuator
int16_t input[NUM_MOTORS];  // intermediate input retrieved from the serial buffer
uint16_t desired_pos[NUM_MOTORS]; // desired (user-inputted and validated) position of each actuator

// Feedback variables
int16_t pos_diff;  // difference between current and desired position
int16_t previous_diff[NUM_MOTORS];  // last position difference for each actuator; for derivative gain
int16_t total_diff[NUM_MOTORS];  // cumulative position difference for each actuator; for integral gain
uint8_t previous_inst[NUM_MOTORS];  // starting/past sample instance for each actuator; for derivative gain
uint8_t current_inst[NUM_MOTORS];  // new/incrementing sample instance for each actuator; for derivative gain
float p_corr;  // proportional correction for PID
float i_corr;  // integral correction for PID
float d_corr; // differential correction for PID
float corr;  // final feedback PWM value from PID correction

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
        total_diff[motor] = 0;
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
    normalOp();
}

/*
 * Read and parse serial input into actuator positions.
 */
void readSerial()
{
    // Parse ints from serial
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        input[motor] = SerialUSB.parseInt();
    }

    // Check that inputs are valid
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        if (input[motor] < MIN_POS || input[motor] > MAX_POS)
        {
            return;
        }
    }

    // Set the input as the desired positions
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        total_diff[motor] = 0;  // reset integral feedback given a new target
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
        // Compute the error value
        pos_diff = pos[motor] - desired_pos[motor];

        // Compute error-dependent PID variables
        total_diff[motor] += pos_diff;
        if (previous_diff[motor] != pos_diff)
        {
            previous_inst[motor] = current_inst[motor];
            current_inst[motor] = 1;
        }

        // Compute PID gain values
        p_corr = P_COEFF * pos_diff;
        i_corr = I_COEFF * total_diff[motor];
        d_corr = D_COEFF * ((float) pos_diff - previous_diff[motor]) / (current_inst[motor] + previous_inst[motor]);

        // Compute the correction value and set the direction and PWM for the actuator
        corr = p_corr + i_corr + d_corr;
        dir[motor] = (corr > 0) ? RETRACT : EXTEND;
        pwm[motor] = constrain(abs(corr), MIN_PWM, MAX_PWM);

        // Send the direction and PWM commands
        digitalWrite(DIR_PINS[motor], dir[motor]);
        analogWrite(PWM_PINS[motor], pwm[motor]);

        // Increment sample-dependent PID variables for the next sampling time
        current_inst[motor] += 1;
        previous_diff[motor] = pos_diff;
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

        // Stop the extension, get an averaged analog reading
        analogWrite(PWM_PINS[motor], 0);
        max_readings[motor] = getAverageReading(motor);

        // Finish with retraction
        digitalWrite(DIR_PINS[motor], RETRACT);
        analogWrite(PWM_PINS[motor], MAX_PWM);
        delay(RESET_DELAY);

        // Stop the retraction, get an averaged analog reading
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
