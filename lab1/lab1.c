#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <common.h>
#include <menu.h>
#include <util/delay.h>
#include <VirtualSerial.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/*
  Function prototypes
*/
void reset_state();
void print_uptime();
void handle_task_yellow_led();
void handle_task_red_led();

/*
  Flags and Globals
*/
bool menu_release = false;
int normal_first = false;
bool menu_first = false;
uint32_t last_red_cycle = 0;

/*
  ISR Defns
*/
ISR(PCINT0_vect) {
  uint8_t pin_val = PINB & _BV(PB3);
  if (pin_val != 0) {
    // button release
    if (menu_release == false) {
      menu_release = true;
    } else {
      menu_release = false;
    }
  }
}

ISR(TIMER3_COMPA_vect) {
  // Every 40hz
  handle_task_yellow_led();
}

/*
  Helpers
*/
void reset_state() {
  last_red_cycle = 0;
  reset_uptime();
}

void print_uptime() {
  printf("%lu\r\n", uptime_ms);
}

void setup_serial() {
  SetupHardware();
}

void handle_task_yellow_led() {
  uint8_t yelval = PIND & _BV(PD6);
  if (yelval == 0) {
    PORTD |= (1 << PORTD6);  // Turn on
  } else {
    PORTD &= ~(1 << PORTD6); // Turn off
  }
}

void handle_task_red_led() {
  uint32_t current_cycle = system_uptime() / 100;
  if (current_cycle > last_red_cycle) {
    last_red_cycle = current_cycle;
    uint8_t redval = PINB & _BV(PB4);
    if (redval == 0) {
      PORTB |= (1 << PORTB4);  // Turn on
    } else {
      PORTB &= ~(1 << PORTB4); // Turn off
    }
  }
}

/*
  Main and Inits
*/
void init_task_green_led() {
  // Uses timer 1
  DDRB |= (1 << DDB6);
  TCCR1A |= (1 << COM1B1) | (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << CS12) | (1 << CS10);
  ICR1 = 1562;
  OCR1B = ICR1 / 2;
}

void init_task_yellow_led() {
  DDRD |= (1 << DDD6);
  TCCR3A = 0x00;
  TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
  OCR3A = 6250;
  TIMSK3 = (1 << OCIE3A);
}

void init_task_red_led() {
  DDRB |= (1 << DDB4);
}

void halt() {
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR3A = 0;
  TCCR3B = 0;
}

void init_all_tasks() {
  init_1000hz_timer_0();
  init_task_green_led();
  init_task_red_led();
  init_task_yellow_led();
  init_input_buttons();
  sei();
}

void loop_menu() {
  if (menu_first == false) {
    menu_first = true;
    normal_first = false;
    printf("Initializing menu...\r\n");
    halt();
    print_usage();
  }

  handle_menu();
}

void loop_tasks() {
  if (normal_first == false) {
    normal_first = true;
    menu_first = false;
    printf("Resuming program execution...\r\n");
    reset_state();
    init_all_tasks();
  }

  handle_task_red_led();
}

int main() {
  setup_serial();

  while (1) {
    USB_Mainloop_Handler();

    if (menu_release == true) {
      loop_menu();
    } else {
      loop_tasks();
    }
  }
}
