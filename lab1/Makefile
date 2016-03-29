PORT=/dev/tty.usb*
MCU=atmega32u4
CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os -std=c99 -I../VirtualSerial/ -I../common/ -I./src/menu/ -I./src/hough/
LDFLAGS=-Wl,-gc-sections -Wl,-relax -L../VirtualSerial/ -lVirtualSerial
CC=avr-gcc
TARGET=lab1
OBJECT_FILES=./src/lab1.o ../common/common.o ./src/menu/menu.o ./src/hough/hough.o

all: $(TARGET).hex

clean:
	rm -f *.o *.hex *.obj *.hex

%.hex: %.obj
	avr-objcopy -R .eeprom -O ihex $< $@

%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

program: $(TARGET).hex
	avrdude -p $(MCU) -c avr109 -P $(PORT) -U flash:w:$(TARGET).hex
