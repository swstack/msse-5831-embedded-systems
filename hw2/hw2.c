/*

Key datasheet info:

Button A
  Pin = PB3
  Interrupt = PCINT3

Button B
  Pin = PD5

Button C
  Pin = PB0
  Interrupt = PCINT0

*/

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Does not meet functional requirements:
// - need to be toggling LED in main loop.
// - LED should toggle 10 times on release only (not press) using a for-loop delay
// Think about building generic functions, structs, #defines for clear/set/read ACL

int main() {
  // Configure buttons as input
  // Note: "&= ~" is an idiomatic way to put the bit to 0
  DDRB &= ~(1 << DDB3);  // Button A
  DDRB &= ~(1 << DDB0);  // Button C

  // Enable Button A pull-up resistor
  PORTB |= (1 << PORTB3);  // need to enable for Button C too. ACL

  // Pin Change Interrupt Control Register = 1
  // Any change on any enabled PCING7..0 will can an interrupt.
  PCICR = 1;

  // Enable interrupts on Button A (PCINT3) and Button C (PCINT0)
  PCMSK0 = 0b00001000;  // hard-coded bits prone to typos. Did not enable button C. ACL 

  // Configures yellow LED pin as output
  DDRC |= (1 << DDC7);

  // Enable all interrupts
  sei();

  while(1)
  {

  }

  return 0;
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
