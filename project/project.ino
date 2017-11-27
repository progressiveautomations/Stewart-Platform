#include "project.h"

// Turn off the watchdog timer during program startup to avoid timeout after a system reset
uint8_t mcusr_mirror __attribute__ ((section(".noinit")));
void get_mcusr(void) __attribute__((naked)) __attribute__((section(".init3")));
void get_mcusr(void)
{
    mcusr_mirror = MCUSR;
    MCUSR = 0;
    wdt_disable();
}


// PWM and direction value variables
uint16_t pwm[NUM_MOTORS];  // maximum of 255, but 16-bit for input consistency
MotorDirection dir[NUM_MOTORS];

// Position variables (extreme values from manual calibration)
uint16_t pos[NUM_MOTORS];
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
long reading_sum;  // sum of multiple readings to be normalized for a final value
uint16_t input_value;  // matched value obtained from the parser
uint16_t input_array[NUM_MOTORS];  // final array of position values from the input
uint16_t pos_diff;  // difference between current and desired position

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
    wdt_enable(WDTO_1S);  // allow the watchdog timer to reset the board if it hangs for >1s

    // Initialize SPI configuration 
    SPI.begin();
    SPI.setBitOrder(BIT_ORDER);
    SPI.setDataMode(DATA_MODE);
    previous_time = 0; 

    // Initialize the pins for each actuator (slave select, direction, PWM)
    for (motor = 0; motor < NUM_MOTORS; ++motor)
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
    for (motor = 0; motor < NUM_MOTORS; ++motor)
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
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        desired_pos[motor] = 0;
        pwm[motor] = 0;
    }

    // @TODO:
    // Calculate actuator lengths? Automatic calibration?

    // Configure threads for serial input processing
    input_thread.setInterval(INPUT_INTERVAL);
    parser_thread.setInterval(PARSER_INTERVAL);
    translator_thread.setInterval(TRANSLATOR_INTERVAL);

    input_thread.onRun(getInput);
    parser_thread.onRun(parseInput);
    translator_thread.onRun(translateInput);
}


/*
    Execute the loop routine, mainly contained within the threads.
    Also prints platform info at a given interval.
*/
void loop()
{
    wdt_reset();  // reset the watchdog timer (last loop successful)
    controller.run();  // execute the threads

    // // Print position and PWM info
    
}


/*
    Thread function to read and store serial input.
    Also manages the buffer queue size.
*/
void getInput()
{
    if (!buffer_locked)
    {
        while (Serial.available() > 0)
        {
            char_queue.add(Serial.read());
        }

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
    if (!buffer_locked && char_queue.size() > INPUT_TRIGGER)
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
        char target_buf[target_len + 1];
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
        else
        {
            // Print the failed result for debug
            Serial.print("Unable to properly parse input: ");
            Serial.print(target_buf);
            Serial.println("");
        }

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
        // Normalize the value over multiple readings
        reading_sum = 0;
        for (reading = 0; reading < NUM_READINGS; ++reading)
        {
            reading_sum += analogRead(POT_PINS[motor]);
        }

        pos[motor] = map(reading_sum / NUM_READINGS,
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

    // Check actuator positions and set movement parameters (PWM, direction) as needed
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        pos_diff = pos[motor] - desired_pos[motor];
        at_correct_pos[motor] = (pos_diff == 0);

        if (at_correct_pos[motor])
        {
            pwm[motor] = 0;
        }
        else
        {
            // Set a slower PWM if the actuator is close to the desired position
            if (pos_diff <= POSITION_TOLERANCE)
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
                dir[motor] = EXTEND;
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
void printMotorInfo(uint16_t pins[])
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
    Moves all actuators in a given direction.

    @param dir: direction (EXTEND/RETRACT) for the actuator
*/
void moveAll(MotorDirection dir)
{
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        digitalWrite(DIR_PINS[motor - 1], dir);
        analogWrite(PWM_PINS[motor - 1], MAX_POS);
    }
}

/*
    Moves one actuator in a given direction.
    Prints the raw analogRead position for the call.

    @param motor: actuator motor number to move
    @param dir: direction (EXTEND/RETRACT) for the actuator
*/
void moveOne(uint8_t motor, MotorDirection dir)
{
    digitalWrite(DIR_PINS[motor - 1], dir);
    analogWrite(PWM_PINS[motor - 1], MAX_POS);
    Serial.println(analogRead(POT_PINS[motor - 1]));
}

/*
    Calibration routine for all actuators.

    @TODO: finish implementation.
*/
void calibrateAll()
{
    // Read potentiometer positions
    for (motor = 0; motor < NUM_MOTORS; ++motor)
    {
        pos[motor] = analogRead(POT_PINS[motor]);
    }
    printMotorInfo(pos);
}
