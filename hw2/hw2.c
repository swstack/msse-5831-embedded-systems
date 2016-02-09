/*

Key datasheet info:

Button A
  Pin = PB3
  Interrupt = PCINT3

Button B
  Pin = PD5

Button C
  Pin = PB0

*/

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Button A = PB3 (PCMSK = PCINT3)
// Button B = PD5
// Button C = PB0

int main() {
  // Configure buttons as input
  // Note: "&= ~" is an idiomatic way to put the bit to 0
  DDRB &= ~(1 << DDB3);  // Button A
  DDRB &= ~(1 << DDB0);  // Button C

  // Enable Button A pull-up resistor
  PORTB |= (1 << PORTB3);

  // Pin Change Interrupt Control Register = 1
  // Any change on any enabled PCING7..0 will can an interrupt.
  PCICR = 1;

  // Enable interrupts on Button A (PCINT3) and Button C (PCINT0)
  PCMSK0 = 0b00001000;

  // Configures yellow LED pin as output
  DDRC |= (1 << DDC7);

  // Enable all interrupts
  sei();

  while(1)
  {

  }
}

void flashYellow() {
  PORTC = (1 << PORTC7);
  _delay_ms(1000);
  PORTC = 0;
  _delay_ms(1000);
}

ISR(PCINT0_vect) {
  flashYellow();
}
