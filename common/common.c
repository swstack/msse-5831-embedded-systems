#include <avr/interrupt.h>
#include "common.h"

// Global variables
uint32_t uptime_ms = 0;
led_state led_state_green = OFF;
led_state led_state_yellow = OFF;
led_state led_state_red = OFF;

ISR(TIMER0_COMPA_vect) {
  // Every 1000hz
  uptime_ms++;
}

void init_input_buttons() {
  // Initialize Button A as input
  DDRB &= ~(1 << DDB3);

  // Enable Button A pull-up resistor
  PORTB |= (1 << PB0) | (1 << PB3);

  // Pin Change Interrupt Control Register = 1
  // Any change on any enabled PCINT7..0 will can an interrupt.
  PCICR = 1;
  PCMSK0 = (1 << PCINT3) | (1 << PCINT0);
}

void init_on_board_leds() {
  // Configure on-board LED pins to output
  DDRC |= (1 << DDC7);  // yellow
  DDRD |= (1 << DDD5);  // green
  DDRB |= (1 << DDB0);  // red
}

void init_1hz_timer_3() {
  // Setup CTC mode (Clear Timer Compare)
  TCCR3A = 0;  // Bits 0 and 1 to value 0

  // Setup prescaler value to 1024
  TCCR3B = (1 << CS30) | (1 << CS32) | (1 << WGM32);

  // Set TOP rollover to 15625
  OCR3A = 15625;

  // Enable Output Compare A Match Interrupt
  TIMSK3 = (1 << OCIE3A);
}

void init_1000hz_timer_3() {
  // Setup CTC mode (Clear Timer Compare)
  TCCR3A = 0;  // Bits 0 and 1 to value 0

  // Setup prescaler value to 64
  TCCR3B = (1 << CS30) | (1 << CS31) | (1 << WGM32);

  // Set TOP rollover to 250
  OCR3A = 250;

  // Enable Output Compare A Match Interrupt
  TIMSK3 = (1 << OCIE3A);
}

void init_1000hz_timer_0() {
  // Setup CTC mode (Clear Timer Compare)
  TCCR0A = 0;  // Bits 0 and 1 to value 0

  // Setup prescaler value to 64
  TCCR0B = (1 << CS00) | (1 << CS01) | (1 << WGM02);

  // Set TOP rollover to 250
  OCR0A = 250;

  // Enable Output Compare A Match Interrupt
  TIMSK0 = (1 << OCIE0A);
}

void toggle_on_board_yellow() {
  if (led_state_yellow == ON) {
    yellow(OFF);
  } else {
    yellow(ON);
  }
}

void toggle_on_board_green() {
  if (led_state_green == ON) {
    green(OFF);
  } else {
    green(ON);
  }
}

void toggle_on_board_red() {
  if (led_state_red == ON) {
    red(OFF);
  } else {
    red(ON);
  }
}

void yellow(led_state state) {
  if (state == ON) {
    PORTC |= (1 << PORTC7);  // Set high
  } else {
    PORTC &= ~(1 << PORTC7);  // Set low
  }
  led_state_yellow = state;
}

void green(led_state state) {
  if (state == ON) {
    PORTD &= ~(1 << PORTD5);  // Set low
  } else {
    PORTD |= (1 << PORTD5);  // Set high
  }
  led_state_green = state;
}

void red(led_state state) {
  if (state == ON) {
    PORTB &= ~(1 << PORTB0);  // Set low
  } else {
    PORTB |= (1 << PORTB0);  // Set high
  }
  led_state_red = state;
}

void gpio_red(led_state state) {
  if (state == ON) {
    PORTD |= (1 << PORTD6);  // Set high
  } else {
    PORTD &= ~(1 << PORTD6);  // Set low
  }
  led_state_red = state;
}

void gpio_green(led_state state) {
  if (state == ON) {
    PORTB |= (1 << PORTB6);  // Set high
  } else {
    PORTB &= ~(1 << PORTB6);  // Set low
  }
  led_state_red = state;
}


uint32_t system_uptime() {
  cli();
  uint32_t cpy = uptime_ms;
  sei();
  return cpy;
}

void reset_uptime() {
  cli();
  uptime_ms = 0;
  sei();
}
