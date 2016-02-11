#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Global variables
uint32_t ms_count = 0;

// Function prototypes
void flashGreen();
void flashYellow();

ISR(TIMER1_COMPA_vect) {
  ms_count++;

  if (ms_count == 1000) {
    ms_count = 0;
  }
}

void initLEDs() {
  // Configures yellow LED pin as output
  DDRC |= (1 << DDC7);

  // Configures green LED as output
  DDRD |= (1 << DDD5);
}

void initTimers() {

  // Setup CTC mode (Clear Timer Compare)
  TCCR1A = 0;  // Bits 0 and 1 to value 0

  // Setup prescaler value
  TCCR1B = (1 << CS10) | (1 << CS11) | (1 << WGM12);

  // Set TOP (OCR1A)
  OCR1A = 250;

  // Enable timer interrupt
  TIMSK1 = (1 << OCIE1A);

}

void flashYellow() {
  PORTC |= (1 << PORTC7);  // Turn on (set high)
  _delay_ms(1000);
  PORTC &= ~(1 << PORTC7);  // Turn off (set low)
}

void flashGreen() {
  PORTD &= ~(1 << PORTD5);  // Turn on (set low)
  _delay_ms(1000);
  PORTD |= (1 << PORTD5);  // Turn off (set high)
}


int main() {
  initTimers();
  initLEDs();
  sei();

  while (1) {
    flashGreen();
    _delay_ms(1000);
  }

  return 0;
}
