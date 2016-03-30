#ifndef _MENU
#define _MENU

#include <stdlib.h>

typedef void (*no_arg_callback)(void);
typedef void (*int_arg_callback)(int arg);

no_arg_callback on_command_print;
no_arg_callback on_command_reset;
no_arg_callback on_command_run_experiment;
int_arg_callback on_command_config_experiment;
int_arg_callback on_command_config_green_led;

void handle_menu();
void print_usage();
void initial_prompt();

#endif
