#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <common.h>
#include <menu.h>
#include <util/delay.h>
#include <VirtualSerial.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/*
  Flags and Globals
*/
bool menuRelease = false;

/*
  ISR Defns
*/
ISR(PCINT0_vect) {
  uint8_t pinVal = PINB & _BV(PB3);
  if (pinVal != 0) {
    // button release
    // TOOD: Xor by itself
    if (menuRelease == false) {
      menuRelease = true;
    } else {
      menuRelease = false;
    }
  }
}

/*
  Helpers
*/

void print_uptime() {
  printf("%lu\r\n", uptime_ms);
}

/*
  Main and Inits
*/
void initGreenLedTask() {
  // Uses timer 1
  DDRB |= (1 << DDB6);
  TCCR1A |= (1 << COM1B1) | (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << CS12) | (1 << CS10);
  ICR1 = 1562;
  OCR1B = ICR1 / 2;
}

void init() {
  SetupHardware();
  init1000hzTimer0();
  initOnBoardLEDs();
  initGreenLedTask();
  initInputButtons();
  sei();
}

int main() {
  init();

  while (1) {
    USB_Mainloop_Handler();

    if (menuRelease == true) {
      handleMenu();
    } else {

    }
  }
}
