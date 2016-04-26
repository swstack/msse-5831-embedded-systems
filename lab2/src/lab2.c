#include <avr/io.h>
#include <VirtualSerial.h>
#include <stdlib.h>
#include <common.h>
#include <stdbool.h>
#include <menu.h>

#define OCR_MAX 30000
#define OCR_MIN 2000
#define ENCODER_FULL_ROTATION 2249

#define INSTRUCTION_TYPE_DELAY 11
#define INSTRUCTION_TYPE_MOTOR 12

typedef struct {
    int type;
    int data;
} instruction_t;

/************************************************
  Globals
*************************************************/

uint32_t encoder_frequency = 0;  // Not really used, just for testing

// Motor state
int32_t encoder = 0;
int32_t setpoint = 0;
int32_t error = 0;
int zero_error_counter = 0;
bool position_good = false;
bool motor_instruction_running = false;
bool delay_instruction_running = false;

// Gains
int kp = 0;
int kd = 0;

// Instruction state
bool instructions_complete = false;
int current_instruction = 0;

// Delay state
uint32_t delay_uptime = 0;

/************************************************
  Helplers
*************************************************/

void set_duty_raw(int raw_ocr) {

  int ocr_value;
  if (raw_ocr >= OCR_MAX) {
    ocr_value = OCR_MAX;
  } else if (raw_ocr <= OCR_MIN) {
    ocr_value = OCR_MIN;
  } else {
    ocr_value = raw_ocr;
  }

  OCR1B = ocr_value;
}

void set_duty_percent(int percent) {
  // Set the duty cycle given a percent 1-100. This effectively changes the
  // speed of the motor.
  //

  int ocr_value;
  if (percent >= 100) {
    ocr_value = OCR_MAX;
  } else if (percent <= 0) {
    ocr_value = 0;
  } else {
    ocr_value = (percent * OCR_MAX) / 100;
  }

  OCR1B = ocr_value;
}

void forward() {
  // Tell the motor to go forward
  //

  PORTE &= ~(_BV(PORTE2));
}

void reverse() {
  // Tell the motor to go reverse
  //

  PORTE |= _BV(PORTE2);
}

void update_pid() {
  // Logic for the PID controller, manages direction and torque of motor
  //

  // Calculate error
  error = setpoint - encoder;

  // Handle direction
  if (error > 0) {
    forward();
  } else if (error < 0) {
    reverse();
  } else {
    zero_error_counter++;
  }

  // Debouncing! Make sure the error is zero for an extended period of time
  if (zero_error_counter > 10) {
    position_good = true;

  } else {

    // Handle torque/duty
    if (error != 0) {
      kp = 30;
      set_duty_raw(abs(kp * error));
    } else {
      set_duty_percent(0);
    }
  }

}

int setpoint_for_degrees(int total_degrees) {
  // Translate degrees (rotations) into a setpoint
  //
  // A setpoint is an integer that defines the allowable number of encoder
  // "ticks" that may occur until the setpoint has been reached.
  //

  int total_encoder_ticks = 0;
  int full_rotations = total_degrees / 360;
  int partial_rotation = total_degrees % 360;
  total_encoder_ticks += full_rotations * ENCODER_FULL_ROTATION;
  total_encoder_ticks += ((uint32_t) ENCODER_FULL_ROTATION * partial_rotation) / 360;
  return total_encoder_ticks;
}

/************************************************
  Inits
*************************************************/

void init_motor() {
  // Initialize the PWM signal that controls the motor duty cycle
  //

  // Configure header pin 10 (motor) as output
  DDRB |= (1 << DDB6);

  // Timer 1: phase-correct PWM mode
  TCCR1A |= (1 << COM1B1) | (1 << WGM11);
  TCCR1B |= (1 << WGM13);

  // Prescaler settings, comment out as needed
  TCCR1B |= (1 << CS10);                      // prescaler 1
  // TCCR1B |= (1 << CS11);                      // prescaler 8
  // TCCR1B |= (1 << CS11) | (1 << CS10);        // prescaler 64
  // TCCR1B |= (1 << CS12);                      // prescaler 256
  // TCCR1B |= (1 << CS12) | (1 << CS10);        // prescaler 1024

  ICR1 = OCR_MAX;
}

void init_encoder() {
  // Initialize the pin change interrupt that the encoder loop requires
  //

  DDRD &= ~(_BV(DDB4) | _BV(DDB5));
  PCMSK0 = _BV(PCINT5) | _BV(PCINT4);
  PCICR =_BV(PCIE0);
}

void init_control_loop_timer() {
  // Initialize a timer interrupt that will act as the PID control loop
  //

  TCCR0A = _BV(WGM01);
  TCCR0B = (1 << CS00) | (1 << CS01) | (1 << WGM02);
  OCR0A = 250;
  TIMSK0 = _BV(OCIE0A);
  TCNT0 = 0;
}

void init_serial() {
  // Initialize the lufa library
  //

  SetupHardware();
  sei();
}

void init() {
  // Initialize the system
  //

  init_on_board_leds();
  init_control_loop_timer();
  init_1000hz_timer_3();
  init_encoder();
  init_motor();
}

/************************************************
  Main
*************************************************/
void handle_delay(int target_delay) {
  // Start or handle a delay instruction, called from the main loop
  //

  if (delay_instruction_running == false) {
      printf("Starting new Delay Instruction (%dms)...\r\n", target_delay);

      delay_uptime = uptime_ms + target_delay;
      delay_instruction_running = true;

    } else {

      int diff = delay_uptime - uptime_ms;
      if (diff < 0) {

        printf("Delay Instruction complete!\r\n");
        delay_instruction_running = false;

        current_instruction++;
      }
    }
}


void handle_motor(int target_setpoint) {
  // Start or handle a motor instruction, called from the main loop
  //

  if (motor_instruction_running == false) {

    position_good = false;
    zero_error_counter = 0;
    // encoder = 0;
    setpoint += target_setpoint;
    error = setpoint - encoder;
    printf("Starting new Motor Instruction (Setpoint = %ld) (Error = %ld)\r\n", setpoint, error);

    // Start the motor!
    motor_instruction_running = true;

  } else {

    if (position_good == true) {

      printf("Motor Instruction complete!\r\n");
      motor_instruction_running = false;

      current_instruction++;
    }

  }
}

void handle_instruction(instruction_t instruction) {
  // Handle the current instruction
  //

  switch(instruction.type) {

    case INSTRUCTION_TYPE_MOTOR :
      handle_motor(instruction.data);
      break;

    case INSTRUCTION_TYPE_DELAY :
      handle_delay(instruction.data);
      break;

    default :
      printf("Unknown instruction type: %d", instruction.type);
      break;
  }

}

void make_instruction(instruction_t *in, int instruction_type, int instruction_data) {
  // Given an instruction pointer, type and data, create an instruction
  //

  in->type = instruction_type;
  in->data = instruction_data;
}

void interpolator(instruction_t *instructions) {
  // Fill the instructions array with instructions
  //
  // Instructions are defined as type instruction_t with a "type" and "data".
  // There are currently only two types of instructions, "delay" and "motor".
  // A delay instruction contains the delay data in the form of milliseconds
  // and motor instuctions contain a positive or negative setpoint.
  //

  make_instruction(&instructions[0], INSTRUCTION_TYPE_MOTOR, setpoint_for_degrees(90));
  make_instruction(&instructions[1], INSTRUCTION_TYPE_DELAY, 2000);
  make_instruction(&instructions[2], INSTRUCTION_TYPE_MOTOR, 0 - setpoint_for_degrees(360));
  make_instruction(&instructions[3], INSTRUCTION_TYPE_DELAY, 2000);
  make_instruction(&instructions[4], INSTRUCTION_TYPE_MOTOR, setpoint_for_degrees(5));
}

int main() {
  // Main loop
  //

  init_serial();
  initial_prompt();

  init();
  flash_on_board_leds();

  instruction_t instructions[5];
  interpolator(instructions);

  while (1) {

    // Handle serial comms
    USB_Mainloop_Handler();

    if (current_instruction > 4) {
      if (instructions_complete == false) {
        printf("All instructions complete!\r\n");
        instructions_complete = true;
      }

    } else {
      handle_instruction(instructions[current_instruction]);
    }

  }

  return 0;
}

/************************************************
  Interrupts
*************************************************/

ISR(TIMER3_COMPA_vect) {
  // Uptime (1000hz)
  //

  uptime_ms++;
}

ISR(TIMER0_COMPA_vect) {
  // PD control loop timer (1000hz)
  //

  update_pid();
}

ISR(PCINT0_vect) {
  // Pin change interrupt for encoder
  //
  // We need to capture the rising and falling edges of both encoder outputs in
  // order to count the 48 ticks per revolution of the drive shaft.
  //

  encoder_frequency++;  // For testing purposes only

  static uint8_t encoder_a = 0;
  static uint8_t encoder_b = 0;

  uint8_t tmp_encoder_a = (PINB & _BV(PB4)) ? 1 : 0;
  uint8_t tmp_encoder_b = (PINB & _BV(PB5)) ? 1 : 0;

  if (tmp_encoder_a ^ encoder_b) {
    encoder++;
  } else if (tmp_encoder_b ^ encoder_a) {
    encoder--;
  }

  encoder_a = tmp_encoder_a;
  encoder_b = tmp_encoder_b;

}
