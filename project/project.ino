// Main Arduino code for a 6-dof Stewart platform.
// Main code hosted at: https://github.com/henrymliu/StewartPlatform
//
#include "project.h"

// PWM and direction value variables
int16_t pwm[NUM_MOTORS];  // maximum of 255, but 16-bit for consistent typing
MotorDirection dir[NUM_MOTORS];

// Position variables (all signed despite specification difference in order for consistent typing)
int16_t pos[NUM_MOTORS];
int16_t desired_pos[NUM_MOTORS];
int16_t pos_diff[NUM_MOTORS];  // difference between current and desired position
uint16_t abs_pos_diff;  // absolute pos_diff for an actuator
uint16_t max_pos_diff;  // maximum abs_pos_diff of all the actuators (i.e. held by leading_motor)

// Thread objects
Thread input_thread = Thread();  // to get data from the RX buffer
Thread parser_thread = Thread();  // to parse data in the RX buffer into input
Thread translator_thread = Thread();  // to translate input into actuator movement
StaticThreadController<3> controller(&input_thread, &parser_thread, &translator_thread);

// Input thread variables
LinkedList<char> char_queue; // actively managed backlog of chars from the RX buffer

// Parser thread variables
MatchState ms;  // object used to match Lua string patterns
String target_string;  // string to store characters from the character queue for matching
uint8_t target_len;  // length of the target string (used to generate a temp buffer)
char match_buf[MAX_BUFFER_SIZE];  // buffer to temporarily store matched values

// Translator thread variables
int32_t reading_sum;  // sum of multiple readings to be normalized for a final value
uint16_t input_value;  // matched value obtained from the parser
uint16_t input_array[NUM_MOTORS];  // final array of position values from the input
uint8_t leading_motor;  // actuator at the farthest distance from desired position
LinkedList<uint8_t> movement_order;  // order to move actuators, starting with the leading motor

// Time variables (for printing)
unsigned long current_time;
unsigned long previous_time;

// Flags to prevent collisions between threads
boolean buffer_locked = false;
boolean input_ready = false;
boolean input_valid = true;

// Calibration variables
boolean do_calibration = false;
int16_t max_readings[NUM_MOTORS];
int16_t min_readings[NUM_MOTORS];

// Iterator variables
uint8_t motor;
uint8_t reading;
uint8_t list_index;


/*
 * Initialize pins and actuators, and set configuration values.
 *
 * @NOTE: LOW = selected/enabled
 *        HIGH = deselected/disabled
 */
void setup()
{
    SerialUSB.begin(BAUD_RATE);
    // while (!SerialUSB);  // disable while not using serial printing

    previous_time = 0;  // set intial time for printing platform info

    // Initialize the pins for each actuator (direction, PWM)
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        pinMode(DIR_PINS[motor], OUTPUT);
        pinMode(PWM_PINS[motor], OUTPUT);
        digitalWrite(PWM_PINS[motor], LOW);
    }

    // Initialize actuator enable switches
    pinMode(ENABLE_MOTORS_1, OUTPUT);
    pinMode(ENABLE_MOTORS_2, OUTPUT);
    digitalWrite(ENABLE_MOTORS_1, HIGH);
    digitalWrite(ENABLE_MOTORS_2, HIGH);

    // Initialize all actuator motors
    digitalWrite(ENABLE_MOTORS_1, LOW);
    digitalWrite(ENABLE_MOTORS_2, LOW);

    // For safety, set initial actuator settings and speed to 0  
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        digitalWrite(DIR_PINS[motor], RETRACT);
        analogWrite(PWM_PINS[motor], MAX_PWM);
    }
    delay(RESET_DELAY);
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        desired_pos[motor] = 0;
        pwm[motor] = 0;
    }

    // Configure threads for serial input processing
    input_thread.setInterval(INPUT_INTERVAL);
    parser_thread.setInterval(PARSER_INTERVAL);
    translator_thread.setInterval(TRANSLATOR_INTERVAL);

    // Check if calibration routine is enabled; else run the threads for the normal routine
    if (do_calibration)
    {
        calibrate();
    }
    else
    {
        input_thread.onRun(getInput);
        parser_thread.onRun(parseInput);
        translator_thread.onRun(translateInput);
    }
}


/*
 * Execute the loop routine, mainly contained within the threads.
 */
void loop()
{
    // Execute the thread routines
    controller.run();

    // Print platform info at a given interval (disabled since the serial handle is taken by the host)
    // current_time = millis();
    // if (current_time - previous_time > PRINT_INTERVAL)
    // {
    //     SerialUSB.println("Desired Positions: ");
    //     printMotorInfo(desired_pos);

    //     SerialUSB.println("Current Positions: ");
    //     printMotorInfo(pos);

    //     SerialUSB.println("PWM Values");
    //     printMotorInfo(pwm);

    //     previous_time = current_time;
    // }
}


/*
 * Thread function to read and store serial input. Also manages the buffer queue size.
 */
void getInput()
{
    if (!buffer_locked)
    {
        // Add characters from the input buffer (stored as a FIFO queue)
        while (SerialUSB.available() > 0)
        {
            char_queue.add(SerialUSB.read());
        }

        // Trim the queue to a certain size (remove oldest entries first)
        if (char_queue.size() > MAX_BUFFER_SIZE)
        {
            buffer_locked = true;
            while (char_queue.size() > MAX_BUFFER_SIZE)
            {
                char_queue.remove(0);
            }
            buffer_locked = false;
        }
    }
}


/*
 * Thread function to parse from an input queue and prep data for the translator thread.
 */
void parseInput()
{
    if (!buffer_locked && char_queue.size() >= INPUT_TRIGGER)
    {
        // Empty the queue into the string
        buffer_locked = true;
        while (char_queue.size() > 0)
        {
            target_string += char_queue.shift();
        }
        buffer_locked = false;

        // Convert string to char array
        target_len = target_string.length();
        char target_buf[target_len + 1];  // extra entry for terminating '\0'
        target_string.toCharArray(target_buf, target_len);

        // Parse and store the input if it is valid
        ms.Target(target_buf);
        char result = ms.Match(TARGET_PATTERN, 0);
        if (result == REGEXP_MATCHED)
        {   
            input_valid = true;
            input_ready = false; // only flag true after input is parsed and valid
            for (motor = 0; motor < NUM_MOTORS; ++motor)
            {
                input_value = atoi(ms.GetCapture(match_buf, motor));
                if (MIN_POS <= input_value <= MAX_POS)
                {
                    input_array[motor] = input_value;
                }
                else
                {
                    input_valid = false;
                    break;
                }
                input_ready = input_valid;
           }
        }

        target_string = "";  // reset for the next iteration
    }
}


/*
 * Thread function to translate processed input to actuator movement.
 */
void translateInput()
{
    // Read potentiometer positions and scale them to the Arduino bounds
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        pos[motor] = map(normalizeAnalogRead(motor),
                         ZERO_POS[motor], END_POS[motor],
                         MIN_POS, MAX_POS);
    }

    // If a valid input is ready for processing, set it as the next desired position
    if (input_ready)
    {
        for (motor = 0; motor < NUM_MOTORS; ++motor)
        {
            desired_pos[motor] = input_array[motor];
        }
    }

    // For each actuator, set movement parameters (direction, PWM) based on position relative to desired
    // Also find the farthest actuator from its desired position (will be set as the first to move)
    max_pos_diff = 0;
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        pos_diff[motor] = pos[motor] - desired_pos[motor];
        abs_pos_diff = abs(pos_diff[motor]);

        // Stop the actuator if it is at (or close enough to) its desired position
        if (abs_pos_diff <= POSITION_NEAR_THRESHOLD)
        {
            pwm[motor] = 0;
        }
        else
        {
            // Set a slower PWM if the actuator is close to the desired position
            if (abs_pos_diff <= POSITION_FAR_THRESHOLD)
            {
                pwm[motor] = PWM_NEAR;
            }
            else
            {
                pwm[motor] = PWM_FAR;
            }

            // Set the direction based on over/under-extension
            if (pos_diff[motor] > 0)
            {
                dir[motor] = RETRACT;
            }
            else
            {
                dir[motor] = EXTEND;
            }
        }

        if (abs_pos_diff > max_pos_diff)
        {
            max_pos_diff = abs_pos_diff;
            leading_motor = motor;
        }
    }

    // Get the actuator movement order (around the platform) based on the largest position differences
    // This is to avoid awkward/jerky movement when lower difference actuators are moved first
    // Direction doesn't matter when differences are equal (i.e. symmetrical/inverse platform orientation)
    if (abs(pos_diff[ADJACENT_MOTORS[leading_motor][1]]) >  // difference in the forward direction
        abs(pos_diff[ADJACENT_MOTORS[leading_motor][0]]))  // difference in the reverse direction
    {
        // Order the actuator movement in the forward direction (by increasing actuator number)
        for (motor = leading_motor; motor < NUM_MOTORS; ++motor)
        {
            movement_order.add(motor);
        }
        for (motor = 0; movement_order.size() < NUM_MOTORS; ++motor)
        {
            movement_order.add(motor);
        }
    }
    else
    {
        // Order the actuator movement in the reverse direction (by decreasing actuator number)
        for (motor = leading_motor; motor >= 0; --motor)
        {
            movement_order.add(motor);
        }
        for (motor = NUM_MOTORS - 1; movement_order.size() < NUM_MOTORS; --motor)
        {
            movement_order.add(motor);
        }
    }

    // Execute actuator motion
    for (list_index = 0; list_index < NUM_MOTORS; ++list_index)
    {
        motor = movement_order.get(list_index);

        digitalWrite(DIR_PINS[motor], dir[motor]);
        analogWrite(PWM_PINS[motor], pwm[motor]);
    }
}


/*
 * Print piece of info for all actuators.
 *
 * @param pins: pin array for the info to print (position, PWM, etc.)
 */
void printMotorInfo(int16_t pins[])
{
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        SerialUSB.print(pins[motor]);
        SerialUSB.print(" ");
    }
    SerialUSB.println("");
}


/*
 * Get an average analogRead value over a certain number of readings
 *
 * @param motor: actuator motor number to move (0-5)
 * @return: average reading (int, may be signed)
 */
int16_t normalizeAnalogRead(uint8_t motor)
{
    reading_sum = 0;
    for (reading = 0; reading < NUM_READINGS; ++reading)
    {
        reading_sum += analogRead(POT_PINS[motor]);
    }
    return reading_sum / NUM_READINGS;
}


/*
 * Calibration routine (prints average reading at the extrema for each actuator).
 * Make sure this is done with the platform disassembled or actuators adjusted to prevent mechanical failure.
 */
void calibrate()
{
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        // Start with extension
        digitalWrite(DIR_PINS[motor], EXTEND);
        analogWrite(PWM_PINS[motor], MAX_PWM);
        delay(RESET_DELAY);

        // Stop the extension, get a normalized analog reading
        analogWrite(PWM_PINS[motor], 0);
        max_readings[motor] = normalizeAnalogRead(motor);

        // Finish with retraction
        digitalWrite(DIR_PINS[motor], RETRACT);
        analogWrite(PWM_PINS[motor], MAX_PWM);
        delay(RESET_DELAY);

        // Stop the retraction, get a normalized analog reading
        analogWrite(PWM_PINS[motor], 0);
        min_readings[motor] = normalizeAnalogRead(motor);

        // Print results
        SerialUSB.print(min_readings[motor]);
        SerialUSB.print(" ");
        SerialUSB.print(max_readings[motor]);
        SerialUSB.print("\n");
    }
}
