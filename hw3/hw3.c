#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

typedef enum { OFF, ON } led_state;

// Function prototypes
void yellow(led_state);
void red(led_state);
void green(led_state);
void toggleGreen();
void toggleYellow();
void toggleRed();
int systemUptime();
void resetUptime();
void initLEDs();
void init1hzTimer();
void init1000hzTimer();

// Global variables
uint32_t uptime_ms = 0;
led_state led_state_green = OFF;
led_state led_state_yellow = OFF;
led_state led_state_red = OFF;

//*******************************************
// Core Program Logic
//*******************************************

ISR(TIMER3_COMPA_vect) {
  // Every 500hz
  toggleGreen();
}

ISR(TIMER1_COMPA_vect) {
  // Every 1000hz
  uptime_ms++;
}

void computationallyExpensiveTask() {}

void loop() {
  if (systemUptime() == 250) {
    toggleYellow();
    resetUptime();
  }
}

int main() {
  init1hzTimer();
  init1000hzTimer();
  initLEDs();
  sei();

  while (1) {
    loop();
  }

  return 0;
}

//*******************************************
// Setup/Init Functions
//*******************************************

void initLEDs() {
  // Configures yellow LED pin as output
  DDRC |= (1 << DDC7);

  // Configures green LED as output
  DDRD |= (1 << DDD5);

  // configure red LED as output
  DDRB |= (1 << DDB0);
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

int systemUptime() {
  cli();
  int cpy = uptime_ms;
  sei();
  return cpy;
}

void resetUptime() {
  cli();
  uptime_ms = 0;
  sei();
}
