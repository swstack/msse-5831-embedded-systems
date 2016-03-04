#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <common.h>
#include <util/delay.h>
#include <VirtualSerial.h>
#include <stdio.h>
#include <string.h>

void init() {
  SetupHardware();
  init1000hzTimer();
  sei();
}

int main() {
  init();

  while (1)
  {
    USB_Mainloop_Handler();
  }
}
