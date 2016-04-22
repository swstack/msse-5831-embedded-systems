#include <avr/io.h>
#include <VirtualSerial.h>
#include <stdlib.h>
#include <common.h>

void forward(int speed)
{
  PORTE |= _BV(PORTE2);
  OCR1B = speed;
}

void reverse(int speed)
{
  PORTE &= ~(_BV(PORTE2));
  OCR1B = speed;
}

void init() {
  init_on_board_leds();

  // Initialize motor
  DDRB |= (1 << DDB6);  // Configure header pin 10 (motor) as output

  // Timer 1: phase-correct PWM mode, prescaler of 1024
  TCCR1A |= (1 << COM1B1) | (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << CS10);

  // Timer rollover @ 10hz
  ICR1 = 0xFFF;
}

int main() {

  init();
  flash_on_board_leds();

  while (1) {

  }

  return 0;
}
