#include <VirtualSerial.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <common.h>

#define COMMAND_PRINT 'p'
#define COMMAND_CONFIG_EXPERIMENT 'e'
#define COMMAND_CONFIG_GREEN_LED 'r'
#define COMMAND_RESET 'z'
#define COMMAND_RUN_EXPERIMENT 'g'

uint8_t buf[10];
uint8_t bufIndex = 0;

void doPrint() {
  printf("Running command: Print\r\n");
}

void doConfigExperiment(int experimentNumber) {
  if (experimentNumber == -1) {return;}
  printf("Running command: Config Experiment (%i)\r\n", experimentNumber);
}

void doConfigGreenLED(int toggleRate) {
  if (toggleRate == -1) {return;}
  printf("Running command: Config Green LED (%i)\r\n", toggleRate);
}

void doReset() {
  printf("Running command: Reset\r\n");
}

void doRunExperiment() {
  printf("Running command: Run Experiment\r\n");
}

int parseIntArg() {
  if (bufIndex <= 1) {
    printf("Command (%c) requires an argument!\r\n", buf[0]);
    return -1;
  }

  if (buf[1] != ' ') {
    printf("Please use a space between command and argument!\r\n");
    return -1;  // Needs to be a space
  }

  char argBuf[bufIndex - 2];
  int argBufIndex = 0;
  for (int i = 2; i <= bufIndex; i++) {
    argBuf[argBufIndex] = (char)buf[i];
    argBufIndex++;
  }

  int arg = atoi(argBuf);
  return arg;
}

void processCommand() {
  switch(buf[0]) {
    case COMMAND_PRINT:
      doPrint();
      break;
    case COMMAND_CONFIG_EXPERIMENT:
      doConfigExperiment(parseIntArg());
      break;
    case COMMAND_CONFIG_GREEN_LED:
      doConfigGreenLED(parseIntArg());
      break;
    case COMMAND_RESET:
      doReset();
      break;
    case COMMAND_RUN_EXPERIMENT:
      doRunExperiment();
      break;
  }

  // Reset index to start processing new command
  bufIndex = 0;
}

void handleMenu() {
  USB_Mainloop_Handler();
  int c = fgetc(stdin);
  if (c == -1) {
    // Not sure why, but when no serial data is sent, the value is 0xFF
    return;
  }

  if (c == '\r') {
    if (bufIndex > 0) {
      processCommand();
    }
  } else {
    buf[bufIndex] = c;
    bufIndex++;
  }
}
