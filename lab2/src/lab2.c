#include <avr/io.h>
#include <VirtualSerial.h>
#include <stdlib.h>
#include <common.h>

int main() {

  init_on_board_leds();
  flash_on_board_leds();

  while (1) {

  }

  return 0;
}
