#include <avr/io.h>
#include <VirtualSerial.h>
#include <stdlib.h>
#include <common.h>

/************************************************
  Helplers
*************************************************/

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

/************************************************
  Inits
*************************************************/

void init_motor() {
  // Configure header pin 10 (motor) as output
  DDRB |= (1 << DDB6);

  // Timer 1: phase-correct PWM mode, prescaler of 1024
  TCCR1A |= (1 << COM1B1) | (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << CS10);
  ICR1 = 0xFFF;
}

void init_encoder() {
  DDRD &= ~(_BV(DDB4) | _BV(DDB5));
  PCMSK0 = _BV(PCINT5) | _BV(PCINT4);
  PCICR =_BV(PCIE0);
}

void init_control_loop_timer() {
  TCCR0A = _BV(WGM01);
  TCCR0B = _BV(CS02) | _BV(CS00);  // prescaler 64
  OCR0A = 255;
  TIMSK0 = _BV(OCIE0A);
  TCNT0 = 0;
}

void init() {
  init_on_board_leds();
  init_control_loop_timer();
  init_encoder();
  init_motor();
}

/************************************************
  Main
*************************************************/

int main() {

  init();
  flash_on_board_leds();

  while (1) {

  }

  return 0;
}

/************************************************
  Interrupts
*************************************************/

ISR(TIMER0_COMPA_vect) {
  // 1000hz control loo timer
}


ISR(PCINT0_vect) {
  // Pin change interrupt for encoder
}
