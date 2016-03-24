#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int main() {

  // Configure GPIO header pins as out
  DDRB |= (1 << DDB5);
  DDRB |= (1 << DDB6);

  // Setup Fast PWM mode for pins OC1A and OC1B
  TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
  TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS12) | (1 << CS10);

  ICR1 = 0xFF;
  OCR1A = ICR1 / 2;

  while(1) {};
  return 0;
}
