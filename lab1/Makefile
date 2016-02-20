PORT=/dev/tty.usb*
MCU=atmega32u4
INCLUDES=../VirtualSerial/
CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os -std=c99 -I$(INCLUDES)
LDFLAGS=-Wl,-gc-sections -Wl,-relax -L$(INCLUDES) -lVirtualSerial
CC=avr-gcc
TARGET=lab1
OBJECT_FILES=lab1.o

all: $(TARGET).hex

clean:
	rm -f *.o *.hex *.obj *.hex

%.hex: %.obj
	avr-objcopy -R .eeprom -O ihex $< $@

%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

program: $(TARGET).hex
	avrdude -p $(MCU) -c avr109 -P $(PORT) -U flash:w:$(TARGET).hex
