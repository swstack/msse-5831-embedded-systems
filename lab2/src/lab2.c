#include <avr/io.h>
#include <VirtualSerial.h>
#include <stdlib.h>
#include <common.h>
#include <stdbool.h>
#include <menu.h>

#define DUTY_MAX 30000
#define ENCODER_FULL_ROTATION 2249

/************************************************
  Globals
*************************************************/

uint32_t encoder_frequency = 0;  // Not really used, just for testing
int32_t encoder = 0;
int32_t setpoint = 0;
uint32_t error = 0;
bool motor_instruction_running = false;
bool delay_instruction_running = false;
int current_instruction = 0;
uint32_t delay_uptime = 0;

/************************************************
  Helplers
*************************************************/

void set_duty(int percent) {
  OCR1B = (percent * DUTY_MAX) / 100;
}

void forward() {
  PORTE &= ~(_BV(PORTE2));
}

void reverse() {
  PORTE |= _BV(PORTE2);
}

void stop_motor() {
  set_duty(0);
}

int setpoint_for_degrees(int total_degrees) {
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

  ICR1 = DUTY_MAX;
}

void init_encoder() {
  DDRD &= ~(_BV(DDB4) | _BV(DDB5));
  PCMSK0 = _BV(PCINT5) | _BV(PCINT4);
  PCICR =_BV(PCIE0);
}

void init_control_loop_timer() {
  TCCR0A = _BV(WGM01);
  TCCR0B = (1 << CS00) | (1 << CS01) | (1 << WGM02);
  OCR0A = 250;
  TIMSK0 = _BV(OCIE0A);
  TCNT0 = 0;
}

void init_serial() {
  SetupHardware();
  sei();
}

void init() {
  init_on_board_leds();
  init_control_loop_timer();
  init_1000hz_timer_3();
  init_encoder();
  init_motor();
}

/************************************************
  Main
*************************************************/

void handle_instruction(int index, int instruction_data) {
  // If the index is an even number, the instruction is a motor setpoint. If
  // the index is odd, the instruction is a delay.

  if (index % 2 == 0) {
    // Handle Motor Instruction

    if (motor_instruction_running == false) {
      printf("Starting new Motor Instruction (%d)...\r\n", index);

      motor_instruction_running = true;
      setpoint = instruction_data;
      encoder = 0;

      if (setpoint > 0) {
        printf("New direction: Forward\r\n");
        forward();
      } else {
        printf("New direction: Reverse\r\n");
        reverse();
      }

      printf("New setpoint: %ld, Current Error: %ld\r\n", setpoint, error);
    } else {

      if (error == 0) {
        printf("Motor Instruction (%d) complete!\r\n", index);

        setpoint = 0;
        motor_instruction_running = false;
        current_instruction++;
      }

    }

  } else {
    // Handle Delay Instruction

    if (delay_instruction_running == false) {
      printf("Starting new Delay Instruction (%d)...\r\n", index);

      delay_instruction_running = true;
      delay_uptime = uptime_ms + instruction_data;

    } else {
      int diff = delay_uptime - uptime_ms;
      if (diff < 0) {
        printf("Delay Instruction (%d) complete!\r\n", index);

        delay_instruction_running = false;
        current_instruction++;
      }
    }

  }
}

int* interpolator() {
  // Return a set of instructions for the main loop to execute
  //
  // Instructions are in the form of an array where each element in the array
  // is an integer. Elements with an even index are motor setpoints and elements
  // with an odd index are delays in milliseconds.
  //

  static int instructions[] = {0, 0, 0, 0, 0};
  instructions[0] = setpoint_for_degrees(90);            // Forward 90
  instructions[1] = 500;                                 // Delay 500ms
  instructions[2] = 0 - setpoint_for_degrees(360);       // Reverse 360
  instructions[3] = 500;                                 // Delay 500ms
  instructions[4] = setpoint_for_degrees(5);
  return instructions;
}

int main() {

  init_serial();
  initial_prompt();

  init();
  flash_on_board_leds();

  int *instructions;
  instructions = interpolator();

  while (1) {

    // Handle serial comms
    USB_Mainloop_Handler();

    if (current_instruction > 4) {
      // There are only 5 instructions
      printf("Done!\r\n");
      stop_motor();
      break;
    }

    // Handle motor
    handle_instruction(current_instruction, instructions[current_instruction]);

  }

  USB_Mainloop_Handler();
  return 0;
}

/************************************************
  Interrupts
*************************************************/

ISR(TIMER3_COMPA_vect) {
  // Uptime (1000hz)

  uptime_ms++;
}

ISR(TIMER0_COMPA_vect) {
  // PD control loop timer (1000hz)

  error = setpoint - encoder;
  if (error != 0) {
    set_duty(10);
  }

}

ISR(PCINT0_vect) {
  // Pin change interrupt for encoder
  //
  // We need to capture the rising and falling edges of both encoder outputs in
  // order to count the 48 ticks per revolution of the drive shaft.
  //

  encoder_frequency++;

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
