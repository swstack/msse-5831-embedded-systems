#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <common.h>
#include <util/delay.h>
#include <VirtualSerial.h>
#include <stdio.h>
#include <string.h>


void init()
{
    SetupHardware();
    sei();
}

int main()
{
    init();
    uint8_t value = 0xFF;

    while (1)
    {
        USB_Mainloop_Handler();

        printf("[%p] %x\r\n", &value, value--);
        _delay_ms(500);
    }
}
