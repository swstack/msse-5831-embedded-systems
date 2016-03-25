# Developers Guide

How to run this code on the Atmega-32U4.

## Compile and Upload

```sh
make program
```

## Serial Port

Assuming you are using the VirtualSerial library included in the repo you can use `screen` or any other serial console to read and write ASCII data to the MCU.

In the code:

```
SetupHardware();
sei();

while (1) {
  USB_Mainloop_Handler();
}
```

From your laptop:

```
screen /dev/tty.usb*
```

Note: to quite the screen session push `ctrl + a + \`
