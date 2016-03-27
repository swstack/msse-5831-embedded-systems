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
uint8_t buf_index = 0;

void do_print() {
  printf("Running command: Print\r\n");
}

void do_config_experiment(int experiment_number) {
  if (experiment_number == -1) {return;}
  printf("Running command: Config Experiment (%i)\r\n", experiment_number);
}

void do_config_green_led(int toggle_rate) {
  if (toggle_rate == -1) {return;}
  printf("Running command: Config Green LED (%i)\r\n", toggle_rate);
}

void do_reset() {
  printf("Running command: Reset\r\n");
}

void do_run_experiment() {
  printf("Running command: Run Experiment\r\n");
}

int parse_int_arg() {
  if (buf_index <= 1) {
    printf("Command (%c) requires an argument!\r\n", buf[0]);
    return -1;
  }

  if (buf[1] != ' ') {
    printf("Please use a space between command and argument!\r\n");
    return -1;  // Needs to be a space
  }

  char arg_bug[buf_index - 2];
  int arg_bug_index = 0;
  for (int i = 2; i <= buf_index; i++) {
    arg_bug[arg_bug_index] = (char)buf[i];
    arg_bug_index++;
  }

  int arg = atoi(arg_bug);
  return arg;
}

void process_command() {
  switch(buf[0]) {
    case COMMAND_PRINT:
      do_print();
      break;
    case COMMAND_CONFIG_EXPERIMENT:
      do_config_experiment(parse_int_arg());
      break;
    case COMMAND_CONFIG_GREEN_LED:
      do_config_green_led(parse_int_arg());
      break;
    case COMMAND_RESET:
      do_reset();
      break;
    case COMMAND_RUN_EXPERIMENT:
      do_run_experiment();
      break;
  }

  // Reset index to start processing new command
  buf_index = 0;
}

void print_usage() {
  char *usage = "Menu Options:\r\n"
                "p  : Print data collected for experiment, including job releases and missed deadlines.\r\n"
                "e #: Set-Up this experiment number (e.g. Set flags to activate/deactivate delays in ISRs).\r\n"
                "r #: Set the toggle period of the GREEN LED Task to # ms.\r\n"
                "z  : Reset all variables to set up for a new experiment.\r\n"
                "g  : Go signal for start experiment.\r\n";
  printf(usage);
}

void initial_prompt() {
  uint32_t cycles = 0;
  while(1) {
    USB_Mainloop_Handler();
    printf("Press any key to continue...\r\n");
    if (fgetc(stdin) != -1) {
      return;
    }
    _delay_ms(1000);
  }
}

void handle_menu() {
  int c = fgetc(stdin);
  if (c == -1) {
    // Not sure why, but when no serial data is sent, the value is 0xFF
    return;
  }

  if (c == '\r') {
    if (buf_index > 0) {
      process_command();
    }
  } else {
    buf[buf_index] = c;
    buf_index++;
  }
}
