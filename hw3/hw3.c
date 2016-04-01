/*
Behavior mostly good. Intent was to interleave computationally intensive task with toggling. Polling for 
button press a little too slow (missing presses).
Code is looking "generalized" - that's good - but start using more inline and/or #define for efficiency.
More structs for generalization.
Minor point: freq = 16M/(N*(OCRn-1), thus OCR=249, not 250.
Easy to follow code. Organization pretty good. ACL
*/


#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define LOOP_COUNT_1MS  2000
#define BusyWait_1ms { for ( uint32_t  __i = 0; __i < LOOP_COUNT_1MS; __i++ ) \
{ __asm__ __volatile__ (“nop”); } }

typedef enum { OFF, ON } led_state;

// Function prototypes
void yellow(led_state);
void red(led_state);
void green(led_state);
void toggleGreen();
void toggleYellow();
void toggleRed();
int system_uptime();
void reset_uptime();
void initLEDs();
void init1hzTimer();
void init1000hzTimer();
void initButtons();
void gpio_red(led_state);
void gpio_green(led_state);

// Global variables
uint32_t uptime_ms = 0;
bool task_release_flag = false;
led_state led_state_green = OFF;
led_state led_state_yellow = OFF;
led_state led_state_red = OFF;

//*******************************************
// Core Program Logic
//*******************************************

ISR(TIMER3_COMPA_vect) {
  // Every 500hz
  toggleGreen();

  // Check Button A pressed
  if ((PINB & (1 << PORTB3)) == 0) {
    _delay_ms(200);
    // Still pressed
    if ((PINB & (1 << PORTB3)) == 0) {
      task_release_flag = true;
    }
  }
}

ISR(TIMER1_COMPA_vect) {
  // Every 1000hz
  uptime_ms++;
}

void computationallyExpensiveTask() {
  gpio_red(ON);
  _delay_ms(300);
  gpio_red(OFF);
  gpio_green(ON);
  _delay_ms(500);
  gpio_green(OFF);
  gpio_red(ON);
  _delay_ms(400);
  gpio_red(OFF);
}

void loop() {
  if (system_uptime() >= 250) {
    toggleYellow();
    reset_uptime();
  }

  if (task_release_flag == true) {
    computationallyExpensiveTask();
    task_release_flag = false;
  }
}

int main() {
  init1hzTimer();
  init1000hzTimer();
  initLEDs();
  initButtons();
  sei();

  while (1) {
    loop();
  }

  return 0;
}

//*******************************************
// Setup/Init Functions
//*******************************************
void initButtons() {
  // Initialize Button A as input
  DDRB &= ~(1 << DDB3);

  // Enable Button A pull-up resistor
  PORTB |= (1 << PORTB3);
}

void initLEDs() {
  // Configure on-board LED pins
  DDRC |= (1 << DDC7);  // yellow
  DDRD |= (1 << DDD5);  // green
  DDRB |= (1 << DDB0);  // red

  // Configure GPIO header pins
  DDRD |= (1 << DDD6);  // red
  DDRB |= (1 << DDB6);  // green
}

void init1hzTimer() {
  // Setup CTC mode (Clear Timer Compare)
  TCCR3A = 0;  // Bits 0 and 1 to value 0

  // Setup prescaler value to 1024
  TCCR3B = (1 << CS30) | (1 << CS32) | (1 << WGM32);

  // Set TOP rollover to 15625
  OCR3A = 15625;

  // Enable Output Compare A Match Interrupt
  TIMSK3 = (1 << OCIE3A);
}

void init1000hzTimer() {
  // Setup CTC mode (Clear Timer Compare)
  TCCR1A = 0;  // Bits 0 and 1 to value 0

  // Setup prescaler value to 64
  TCCR1B = (1 << CS10) | (1 << CS11) | (1 << WGM12);

  // Set TOP rollover to 250
  OCR1A = 250;

  // Enable Output Compare A Match Interrupt
  TIMSK1 = (1 << OCIE1A);
}

//*******************************************
// Helper Functions
//*******************************************

void toggleYellow() {
  if (led_state_yellow == ON) {
    yellow(OFF);
  } else {
    yellow(ON);
  }
}

void toggleGreen() {
  if (led_state_green == ON) {
    green(OFF);
  } else {
    green(ON);
  }
}

void toggleRed() {
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

int system_uptime() {
  cli();
  int cpy = uptime_ms;
  sei();
  return cpy;
}

void reset_uptime() {
  cli();
  uptime_ms = 0;
  sei();
}
