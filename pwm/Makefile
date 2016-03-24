PORT=/dev/tty.usb*
MCU=atmega32u4
CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os
LDFLAGS=-Wl,-gc-sections -Wl,-relax
CC=avr-gcc
TARGET=pc_pwm
OBJECT_FILES=pc_pwm.o

all: pc_pwm.hex

clean:
	rm -f *.o *.hex *.obj *.hex

%.hex: %.obj
	avr-objcopy -R .eeprom -O ihex $< $@

%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

program_fast: fast_pwm.hex
	avrdude -p $(MCU) -c avr109 -P $(PORT) -U flash:w:fast_pwm.hex

program_pc: pc_pwm.hex
	avrdude -p $(MCU) -c avr109 -P $(PORT) -U flash:w:pc_pwm.hex
