#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int main() {

  // Configure header pin 9 (motor) as output
  DDRB |= (1 << DDB5);

  // Setup Fast PWM mode for pins OC1A and OC1B
  TCCR1A = (1 << COM1A1) | (1 << WGM10) | (1 << WGM11);
  TCCR1B = (1 << CS10) | (1 << WGM12) ;

  ICR1 = 0xFF;
  OCR1A = ICR1;

  while(1) {};
  return 0;
}
