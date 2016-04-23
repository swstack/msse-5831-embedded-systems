PORT=/dev/tty.usb*
MCU=atmega32u4
CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os -std=c99 -I../VirtualSerial/ -I../common/ -I./src/menu/
LDFLAGS=-Wl,-gc-sections -Wl,-relax -L../VirtualSerial/ -lVirtualSerial
CC=avr-gcc
TARGET=lab2
OBJECT_FILES=./src/lab2.o ../common/common.o ./src/menu/menu.o

all: $(TARGET).hex

clean:
	rm -f *.o *.hex *.obj *.hex

%.hex: %.obj
	avr-objcopy -R .eeprom -O ihex $< $@

%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

program: $(TARGET).hex
	avrdude -p $(MCU) -c avr109 -P $(PORT) -U flash:w:$(TARGET).hex
