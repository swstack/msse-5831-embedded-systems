#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int main() {

  // Configure header GPIO mode as OUTPUT
  // - Port D
  // - Pin 12
  DDRB |= (1 << DDB6);

  // Flash led
  PORTB |= (1 << PB6);
  _delay_ms(100);
  PORTB &= ~(1 << PB6);

  // Use Timer 1 (16 bit) to implement Phase Correct PWM

  TCCR1A |= (1 << COM1B1) | (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << CS12) | (1 << CS10);

  ICR1 = 1562;
  // ICR1 = 0x0FFF;
  OCR1B = ICR1 / 2;

  while(1) {};
  return 0;
}
