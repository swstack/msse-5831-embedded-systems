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
  printf("Running command: Print");
}

void doConfigExperiment(int experimentNumber) {
  if (experimentNumber == -1) {return;}
  printf("Running command: Config Experiment");
}

void doConfigGreenLED(int toggleRate) {
  if (toggleRate == -1) {return;}
  printf("Running command: Config Green LED");
}

void doReset() {
  printf("Running command: Reset");
}

void doRunExperiment() {
  printf("Running command: Run Experiment");
}

int parseIntArg() {
  if (buf[1] != ' ') {
    return -1;  // Needs to be a space
  }

  char argBuf[bufIndex - 2];
  for (int i = 2; i <= bufIndex; i++) {
    argBuf[i] = (char)buf[i];
  }

  int arg = atoi(argBuf);
  // sscanf(argBuf, "%d", &arg);
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
  int c = fgetc(stdin);
  if (c == -1) {
    // Not sure why, but when no serial data is sent, the value is 0xFF
    return;
  }

  if (c == '\r') {
    processCommand();
  } else {
    buf[bufIndex] = c;
    bufIndex++;
  }
}
