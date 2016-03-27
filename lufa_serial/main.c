#define F_CPU 16000000

#include <VirtualSerial.h>
#include <avr/interrupt.h>

int main() {
  SetupHardware();
  sei();

  while(1) {
    USB_Mainloop_Handler();
    printf(".");
  }

  return 0;
}
