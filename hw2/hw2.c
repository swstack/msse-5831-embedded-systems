#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Button A = PB3 (PCMSK = PCINT3)
// Button B = PD5
// Button C = PB0

int main()
{
  DDRC |= (1 << DDC7);    // Make pin 13 be an output.
  while(1)
  {
    PORTC |= (1 << PORTC7);   // Turn the LED on.
    _delay_ms(500);
    PORTC &= ~(1 << PORTC7);  // Turn the LED off.
    _delay_ms(500);
  }
}

ISR(PCINT3_vect)
{
// user code here
}
