// Main Arduino code for a 6-dof Stewart platform.
// Written for the Arduino Due.
//
#include "platform.h"

// Actuator variables
uint8_t pwm[NUM_MOTORS];            // current PWM for each actuator
MotorDirection dir[NUM_MOTORS];     // current direction for each actuator (EXTEND or RETRACT)

// Position variables
int16_t pos[NUM_MOTORS];            // current position (measured by analog read) of each actuator
int16_t input[NUM_MOTORS];          // intermediate input retrieved from the serial buffer
uint16_t desired_pos[NUM_MOTORS];   // desired (user-inputted and validated) position of each actuator

// Feedback variables
int16_t pos_diff;                   // difference between current and desired position
int16_t prop_diff;                  // position difference used for proportional gain
int16_t previous_diff[NUM_MOTORS];  // last position difference for each actuator; for derivative gain
int16_t total_diff[NUM_MOTORS];     // cumulative position difference for each actuator; for integral gain
uint8_t previous_inst[NUM_MOTORS];  // starting/past sample instance for each actuator; for derivative gain
uint8_t current_inst[NUM_MOTORS];   // new/incrementing sample instance for each actuator; for derivative gain
float p_corr;                       // proportional correction for PID
float i_corr;                       // integral correction for PID
float d_corr;                       // differential correction for PID
float corr;                         // final feedback PWM value from PID correction

// Calibration variables
int16_t end_readings[NUM_MOTORS];
int16_t zero_readings[NUM_MOTORS];
bool calibration_valid;

// Time variables (for printing)
unsigned long current_time;         // current time (in millis); used to measure difference from previous_time
unsigned long previous_time;        // last recorded time (in millis); measured from execution start or last print

// Iterator/sum variables
uint8_t motor;                      // used to iterate through actuators by their indexing (0 to NUM_MOTORS - 1)
uint8_t reading;                    // used to iterate through analog reads (0 to NUM_READINGS - 1)
int32_t reading_sum;                // sum of multiple readings to be averaged for a final value


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
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        total_diff[motor] = 0;
        desired_pos[motor] = 0;
        pwm[motor] = MIN_PWM;
    }

    // Initialize serial communication
    SerialUSB.begin(BAUD_RATE);

    // Run calibration
    calibration_valid = true;
    calibrate();

    // Initialize the current print interval
    #if ENABLE_PRINT
    {
        previous_time = 0;
    }
    #endif // ENABLE_PRINT
}


/*
 * Run tasks in an infinite loop.
 */
void loop()
{
    // Read current actuator positions, map them based on actuator-based scaling from calibration
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        pos[motor] = map(getAverageReading(motor), ZERO_POS[motor], END_POS[motor], MIN_POS, MAX_POS);
    }

    // Parse new serial input if enough is in the buffer (also sets desired position if input is valid)
    if (SerialUSB.available() > INPUT_TRIGGER)
    {
        readSerial();
    }

    // Print actuator information at a given interval
    #if ENABLE_PRINT
    {
        printOutput();
    }
    #endif // ENABLE_PRINT

    // For each actuator, set movement parameters (direction, PWM) and execute motion
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        move(motor);
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
 * Move all actuators in a given direction at full PWM.
 */
inline void moveAll(MotorDirection dir)
{
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        digitalWrite(DIR_PINS[motor], dir);
        analogWrite(PWM_PINS[motor], MAX_PWM);
    }
}


/*
 * Read and parse serial input into actuator positions.
 */
inline void readSerial()
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
 * Print actuator variables over serial at a given interval.
 */
inline void printOutput()
{
    current_time = millis();
    if (current_time - previous_time > PRINT_INTERVAL)
    {
        // Print instance header
        #if ENABLE_PRINT_HEADERS
        {
            SerialUSB.print("<COM>\n");
        }
        #endif // ENABLE_PRINT_HEADERS

        // Print desired position
        #if PRINT_DESIRED_POS
        {
            #if ENABLE_PRINT_HEADERS
            {
                SerialUSB.print("      DESIRED POS: ");
            }
            #endif // ENABLE_PRINT_HEADERS

            for (motor = 0; motor < NUM_MOTORS; ++motor)
            {
                SerialUSB.print(desired_pos[motor]);
                SerialUSB.print(" ");
            }

            SerialUSB.print("\n");
        }
        #endif // PRINT_DESIRED_POS

        // Print current position
        #if PRINT_CURRENT_POS
        {
            #if ENABLE_PRINT_HEADERS
            {
                SerialUSB.print("      CURRENT POS: ");
            }
            #endif // ENABLE_PRINT_HEADERS

            for (motor = 0; motor < NUM_MOTORS; ++motor)
            {
                SerialUSB.print(pos[motor]);
                SerialUSB.print(" ");
            }

            SerialUSB.print("\n");
        }
        #endif // PRINT_CURRENT_POS

        // Print PWM values
        #if PRINT_PWM
        {
            #if ENABLE_PRINT_HEADERS
            {
                SerialUSB.print("      PWM        : ");  // align values wih other entries
            }
            #endif // ENABLE_PRINT_HEADERS

            for (motor = 0; motor < NUM_MOTORS; ++motor)
            {
                SerialUSB.print(pwm[motor]);
                SerialUSB.print(" ");
            }

            SerialUSB.print("\n");
        }
        #endif // PRINT_PWM

        // Print CR to end message block
        #if ENABLE_PRINT_HEADERS
        {
            SerialUSB.print("\r");
        }
        #endif // ENABLE_PRINT_HEADERS

        previous_time = current_time;
    }
}


/*
 * Compute the PID values given the current actuator position, and use them
 * to move the actuator with new direction/PWM values.
 */
inline void move(uint8_t motor)
{
    // Compute the error value
    pos_diff = pos[motor] - desired_pos[motor];

    // Compute error-dependent PID variables
    if (abs(pos_diff) <= POS_THRESHOLD[motor])
    {
        prop_diff = 0;
        total_diff[motor] = 0;
    }
    else
    {
        prop_diff = pos_diff;
        total_diff[motor] += pos_diff;
    }
    if (previous_diff[motor] != pos_diff)
    {
        previous_inst[motor] = current_inst[motor];
        current_inst[motor] = 1;
    }

    // Compute PID gain values
    p_corr = P_COEFF[motor] * prop_diff;
    i_corr = I_COEFF[motor] * total_diff[motor];
    d_corr = D_COEFF[motor] * ((float) pos_diff - previous_diff[motor]) / (current_inst[motor] + previous_inst[motor]);

    // Compute the correction value and set the direction and PWM for the actuator
    corr = p_corr + i_corr + d_corr;
    dir[motor] = (corr > 0) ? RETRACT : EXTEND;  // direction based on the sign of the error
    pwm[motor] = constrain(abs(corr), MIN_PWM, MAX_PWM);  // bound the correction by the PWM limits

    // Move the actuator with the new direction and PWM values
    digitalWrite(DIR_PINS[motor], dir[motor]);
    analogWrite(PWM_PINS[motor], pwm[motor]);

    // Increment sample-dependent PID variables for the next sampling time
    current_inst[motor] += 1;
    previous_diff[motor] = pos_diff;
}


/**
 * Calibration routine run on startup.
 * Extends all actuators, reads extended values, then retracts all, and reads their retracted values.
 */
inline void calibrate()
{
    // Extend all actuators
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        // Start with extension
        digitalWrite(DIR_PINS[motor], EXTEND);
        analogWrite(PWM_PINS[motor], MAX_PWM);
    }
    delay(RESET_DELAY);

    // Stop the extension, get averaged analog readings
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        analogWrite(PWM_PINS[motor], 0);
        end_readings[motor] = getAverageReading(motor);

        // Check if the motors are powered (reading is valid)
        calibration_valid = (abs(end_readings[motor] - END_POS[motor]) < OFF_THRESHOLD);
        if (!calibration_valid)
        {
            break;
        }
    }
    // Retract all actuators
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        digitalWrite(DIR_PINS[motor], RETRACT);
        analogWrite(PWM_PINS[motor], MAX_PWM);
    }
    delay(RESET_DELAY);

    // Stop the retraction, get averaged analog readings
    if (calibration_valid)
    {
        for (motor = 0; motor < NUM_MOTORS; ++motor)
        {
            analogWrite(PWM_PINS[motor], 0);
            zero_readings[motor] = getAverageReading(motor);

            // Check if the motors are powered (reading is valid)
            calibration_valid = (abs(zero_readings[motor] - ZERO_POS[motor]) < OFF_THRESHOLD);
            if (!calibration_valid)
            {
                break;
            }
        }
    }

    // Set the new calibration values if found to be valid
    if (calibration_valid)
    {
        for (motor = 0; motor < NUM_MOTORS; ++motor)
        {
            END_POS[motor] = end_readings[motor];
            ZERO_POS[motor] = zero_readings[motor];
        }
    }

    // Print the calibration result (new max/min values, or a warning)
    #if ENABLE_PRINT
    {
        if (calibration_valid)
        {
            SerialUSB.print("<COM> Finished calibration:\n");

            // Print minimum positions
            SerialUSB.print("      MIN POS: ");
            for (motor = 0; motor < NUM_MOTORS; ++motor)
            {
                SerialUSB.print(ZERO_POS[motor]);
                SerialUSB.print(" ");
            }
            SerialUSB.print("\n");

            // Print maximum positions
            SerialUSB.print("      MAX POS: ");
            for (motor = 0; motor < NUM_MOTORS; ++motor)
            {
                SerialUSB.print(END_POS[motor]);
                SerialUSB.print(" ");
            }
            SerialUSB.print("\n");

            // Print CR to end message block
            SerialUSB.print("\r");
        }

        else
        {
            // Print an error message for calibration failure
            SerialUSB.print("<COM> Failed calibration (using default values).\n");
            SerialUSB.print("      Please verify that the actuators are powered on.\n\r");
        }
    }
    #endif // ENABLE_PRINT
}
