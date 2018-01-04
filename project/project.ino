#include "project.h"

// PWM and direction value variables
uint16_t pwm[NUM_MOTORS];  // maximum of 255, but 16-bit for input consistency
MotorDirection dir[NUM_MOTORS];

// Position variables (extreme values from manual calibration)
int16_t pos[NUM_MOTORS];
uint16_t desired_pos[NUM_MOTORS];
boolean at_correct_pos[NUM_MOTORS];

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
uint16_t sum_difference;
uint16_t input_value;  // matched value obtained from the parser
uint16_t input_array[NUM_MOTORS];  // final array of position values from the input
int16_t pos_diff;  // difference between current and desired position

// Time variables (for printing)
unsigned long current_time;
unsigned long previous_time;

// Flags to prevent collisions between threads
boolean buffer_locked = false;
boolean input_ready = false;
boolean input_valid = true;

// Iterator variables
uint8_t motor;
uint8_t reading;


/*
    Initialize pins and actuators, and set configuration values.

    @NOTE: LOW = selected/enabled
           HIGH = deselected/disabled
*/
void setup()
{
    Serial.begin(BAUD_RATE);

    previous_time = 0; 

    // Initialize the pins for each actuator (slave select, direction, PWM)
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
    moveAll(RETRACT);
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

    input_thread.onRun(getInput);
    parser_thread.onRun(parseInput);
    translator_thread.onRun(translateInput);

    // calibrateAll();
}


/*
    Execute the loop routine, mainly contained within the threads.
    Also prints platform info at a given interval.
*/
void loop()
{
    controller.run();  // execute the threads
    // printPlatformInfo();
}


/*
    Thread function to read and store serial input.
    Also manages the buffer queue size.
*/
void getInput()
{
    if (!buffer_locked)
    {
        // Add characters from the input buffer (stored as a FIFO queue)
        while (Serial.available() > 0)
        {
            char_queue.add(Serial.read());
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
    Thread function to parse from an input Queue and prep data for the translator thread.
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
                    Serial.println("Invalid input value detected!");
                    break;
                }
                input_ready = input_valid;
           }
        }
        // else
        // {
        //     // Print the failed result for debug
        //     Serial.print("Unable to properly parse input: ");
        //     Serial.print(target_buf);
        //     Serial.println("");
        // }

        target_string = "";  // reset for the next iteration
    }
}


/*
    Thread function to translate processed input to actuator movement.
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

    // sum_difference = 0;
    // for (motor = 0; motor < NUM_MOTORS; ++ motor)
    // {
    //     pos_diff = pos[motor] - desired_pos[motor];
    //     sum_difference += abs(pos_diff);
    // }

    // Check actuator positions and set movement parameters (PWM, direction) as needed
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        pos_diff = pos[motor] - desired_pos[motor];
        at_correct_pos[motor] = (pos_diff == 0);

        if (at_correct_pos[motor] || abs(pos_diff) <= POSITION_NEAR_THRESHOLD)
        {
            pwm[motor] = 0;
        }
        else
        {
            // Set a slower PWM if the actuator is close to the desired position
            if (abs(pos_diff) <= POSITION_FAR_THRESHOLD)
            {
                pwm[motor] = PWM_NEAR;
            }
            else
            {
                pwm[motor] = PWM_FAR;
            }

            // Set the direction based on over/under-extension
            if (pos_diff > 0)
            {
                dir[motor] = RETRACT;
            }
            else
            {
                // dir[motor] = EXTEND;
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
void printMotorInfo(int16_t pins[])
{
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        Serial.print(pins[motor]);
        Serial.print(" ");
    }
    Serial.println("");
}

/*
    Print position and PWM info for all actuators if within a given interval. 
*/
void printPlatformInfo()
{
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
}

/*
    Get an average analogRead value over a certain number of readings

    @param motor: actuator motor number to move (0-5)
    @return: average reading (int, may be signed)
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
    Moves all actuators in a given direction.

    @param dir: direction (EXTEND/RETRACT) for the actuator
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
    Moves one actuator in a given direction.
    Prints the raw analogRead position for the call.

    @param motor: actuator motor number to move (0-5)
    @param dir: direction (EXTEND/RETRACT) for the actuator
*/
void moveOne(uint8_t motor, MotorDirection dir)
{
    digitalWrite(DIR_PINS[motor], dir);
    analogWrite(PWM_PINS[motor], MAX_PWM);
    Serial.println(analogRead(POT_PINS[motor]));
}

/*
    Calibration routine for all actuators (put it at the end of setup when necessary).
    Make sure this is done with the platform disassembled to prevent mechanical failure.
*/
void calibrateAll()
{
    int16_t max_readings[6];
    int16_t min_readings[6];

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

        // Stop the extension, get a normalized analog reading
        analogWrite(PWM_PINS[motor], 0);
        min_readings[motor] = normalizeAnalogRead(motor);

        // Print results
        Serial.print(min_readings[motor]);
        Serial.print(" ");
        Serial.print(max_readings[motor]);
        Serial.print("\n");
    }
}
