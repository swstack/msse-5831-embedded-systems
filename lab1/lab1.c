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
int normalFirst = false;
bool menuFirst = false;
uint32_t last_red_cycle = 0;

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
void reset_state() {
  last_red_cycle = 0;
  resetUptime();
}

void print_uptime() {
  printf("%lu\r\n", uptime_ms);
}

void setupSerial() {
  SetupHardware();
}

void handleRedLedTask() {
  uint32_t current_cycle = systemUptime() / 100;
  if (current_cycle > last_red_cycle) {
    last_red_cycle = current_cycle;
    uint8_t redval = PINB & _BV(PB4);
    if (redval == 0) {
      PORTB |= (1 << PORTB4);  // Turn on
    } else {
      PORTB &= ~(1 << PORTB4); // Turn off
    }
  }
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

void initYellowLedTask() {
  TCCR3A = 0x00;
  TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
  OCR3A = 6250;
  TIMSK3 = (1 << OCIE3A);
}

void initRedLedTask() {
  DDRB |= (1 << DDB4);
}

void halt() {
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR3A = 0;
  TCCR3B = 0;
}

void initTasks() {
  init1000hzTimer0();
  initGreenLedTask();
  initRedLedTask();
  initInputButtons();
  sei();
}

void menuLoop() {
  if (menuFirst == false) {
    menuFirst = true;
    normalFirst = false;
    printf("Initializing menu...\r\n");
    halt();
    printUsage();
  }

  handleMenu();
}

void normalLoop() {
  if (normalFirst == false) {
    normalFirst = true;
    menuFirst = false;
    printf("Resuming program execution...\r\n");
    reset_state();
    initTasks();
  }

  handleRedLedTask();
}

int main() {
  setupSerial();

  while (1) {
    USB_Mainloop_Handler();

    if (menuRelease == true) {
      menuLoop();
    } else {
      normalLoop();
    }
  }
}
