#include <avr/io.h>
#include <VirtualSerial.h>
#include <stdlib.h>
#include <common.h>
#include <menu.h>

#define DUTY_MAX 30000

/************************************************
  Globals
*************************************************/
uint32_t encoder = 0;
uint32_t encoder_frequency = 0;

/************************************************
  Helplers
*************************************************/

int duty_to_ocr(int duty) {
  return (duty * DUTY_MAX) / 100;;
}

void forward(int duty) {
  PORTE |= _BV(PORTE2);
  OCR1B = duty_to_ocr(duty);
}

void reverse(int duty) {
  PORTE &= ~(_BV(PORTE2));
  OCR1B = duty_to_ocr(duty);
}

/************************************************
  Inits
*************************************************/

void init_motor() {
  // Configure header pin 10 (motor) as output
  DDRB |= (1 << DDB6);

  // Timer 1: phase-correct PWM mode
  TCCR1A |= (1 << COM1B1) | (1 << WGM11);
  TCCR1B |= (1 << WGM13);

  // Prescaler settings, comment out as needed
  TCCR1B |= (1 << CS10);                      // prescaler 1
  // TCCR1B |= (1 << CS11);                      // prescaler 8
  // TCCR1B |= (1 << CS11) | (1 << CS10);        // prescaler 64
  // TCCR1B |= (1 << CS12);                      // prescaler 256
  // TCCR1B |= (1 << CS12) | (1 << CS10);        // prescaler 1024

  //
  ICR1 = DUTY_MAX;
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

void init_serial() {
  SetupHardware();
  sei();
}

void init() {
  init_on_board_leds();
  init_control_loop_timer();
  init_1000hz_timer_3();
  init_encoder();
  init_motor();
}

/************************************************
  Main
*************************************************/

int main() {

  init_serial();
  initial_prompt();

  init();
  flash_on_board_leds();

  forward(100);
  while (1) {
    USB_Mainloop_Handler();
  }

  return 0;
}

/************************************************
  Interrupts
*************************************************/

ISR(TIMER3_COMPA_vect) {
  // 1000hz uptime timer
  uptime_ms++;
}

ISR(TIMER0_COMPA_vect) {
  // 1000hz control loop timer

}

ISR(PCINT0_vect) {
  // Pin change interrupt for encoder
  //
  // We need to capture the rising and falling edges of both encoder outputs in
  // order to count the 48 ticks per revolution of the drive shaft.
  //

  encoder_frequency++;

  static uint8_t encoder_a = 0;
  static uint8_t encoder_b = 0;

  uint8_t tmp_encoder_a = (PINB & _BV(PB4)) ? 1 : 0;
  uint8_t tmp_encoder_b = (PINB & _BV(PB5)) ? 1 : 0;

  if (encoder_a != tmp_encoder_a || encoder_b != tmp_encoder_b) {
    encoder++;
  }

  encoder_a = tmp_encoder_a;
  encoder_b = tmp_encoder_b;
}
