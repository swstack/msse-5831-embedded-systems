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

bool menuRelease = false;

ISR(PCINT0_vect) {
  uint8_t pinVal = PINB & _BV(PB3);
  if (pinVal != 0) {
    // button release
    if (menuRelease == false) {
      menuRelease = true;
    } else {
      menuRelease = false;
    }
  }
}

void init() {
  SetupHardware();
  init1000hzTimer();
  initLEDs();
  initInputButtons();
  sei();
}

int main() {
  init();

  while (1) {
    USB_Mainloop_Handler();

    if (menuRelease == true) {
      handleMenu();
    }
  }
}
