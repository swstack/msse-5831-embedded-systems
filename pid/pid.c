#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <common.h>
#include <avr/interrupt.h>

int main() {
  init_on_board_leds();

  // Configure header pin 10 (motor) as output
  DDRB |= (1 << DDB6);

  // Phase correct PWM mode, prescaler of 1024
  TCCR1A |= (1 << COM1B1) | (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << CS12) | (1 << CS10);

  // Timer rollover @ 10hz
  ICR1 = 1562;

  // PWM/Motor changing state every 100ms
  OCR1B = ICR1 / 2;

  while(1) {
    toggle_on_board_green();
    _delay_ms(300);

  };
  return 0;
}
