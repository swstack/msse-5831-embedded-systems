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
  Flags
*/
bool menuRelease = false;

/*
  ISR Defns
*/
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

/*
  Helpers
*/
void handleGreenLedTask() {

}

/*
  Main and Inits
*/
void initLedTasks() {
  // Configure Green LED GPIO as output
  DDRB |= (1 << DDB6);

  // Green LED Fast PWM config (OC1B)
  TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);

  // Green LED prescaler config
  TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS12) | (1 << CS10);
}

void init() {
  SetupHardware();
  init1000hzTimer();
  initOnBoardLEDs();
  initLedTasks();
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
