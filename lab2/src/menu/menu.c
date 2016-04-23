#include "menu.h"

void initial_prompt() {
  while(1) {
    USB_Mainloop_Handler();
    printf("Press any key to continue...\r\n");
    if (fgetc(stdin) != -1) {
      return;
    }
    _delay_ms(1000);
  }
}
