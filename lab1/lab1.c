#include <avr/io.h>
#include <VirtualSerial.h>
#include <common.h>
#include <menu.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <hough.h>
#include <stdlib.h>

/************************************************
  Function Prototypes
*************************************************/
void reset_state();
void print_uptime();
void handle_task_yellow_led();
void handle_task_red_led();
void handle_task_hough_transform();
void handle_task_green_led_count();
void init_serial();
void init_leds();
void halt();

/************************************************
  Flags/Globals
*************************************************/
bool menu_release = false;
bool jitter_release = false;
int normal_first = false;
bool menu_first = false;
uint32_t last_red_cycle = 0;
uint8_t counter_40hz = 0;
uint8_t green_led_count = 0;

/************************************************
  ISRs
*************************************************/
ISR(PCINT0_vect) {
  uint8_t pin_val = PINB & _BV(PB3);
  if (pin_val != 0) {
    // button release
    if (menu_release == false) {
      menu_release = true;
    } else {
      menu_release = false;
    }
  }
}

ISR(TIMER3_COMPA_vect) {
  // Every 40hz
  if (counter_40hz >= 4) {
    handle_task_yellow_led();
    jitter_release = true;
    // handle_task_hough_transform();
    counter_40hz = 0;
  } else {
    counter_40hz++;
  }
}

ISR(TIMER1_OVF_vect) {
  // PWM timer
  handle_task_green_led_count();
  toggle_on_board_green(); // not part of assignment, just for debug
}

/************************************************
  Helpers
*************************************************/
void halt() {
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR3A = 0;
  TCCR3B = 0;
}

void reset_state() {
  counter_40hz = 0;
  last_red_cycle = 0;
  green_led_count = 0;
  reset_uptime();
}

void set_led_red(led_state state) {
  if (state == ON) {
    PORTB |= (1 << PORTB4);
  } else {
    PORTB &= ~(1 << PORTB4);
  }
}

void set_led_yellow(led_state state) {
  if (state == ON) {
    PORTD |= (1 << PORTD6);
  } else {
    PORTD &= ~(1 << PORTD6);
  }
}

void set_led_green(led_state state) {
  if (state == ON) {
    PORTB |= (1 << PORTB6);
  } else {
    PORTB &= ~(1 << PORTB6);
  }
}

void handle_task_hough_transform() {
  printf("Executing Hough Transform...\r\n");
  volatile char dummyVar;
  uint32_t start = system_uptime();
  dummyVar = houghTransform((uint16_t)&image_red, (uint16_t)&image_green, (uint16_t)&image_blue);
  uint32_t end = system_uptime();
  printf("Hough Transform completed in %lu milliseconds.\r\n", end - start);
}

void handle_task_yellow_led() {
  uint8_t yelval = PIND & _BV(PD6);
  if (yelval == 0) {
    set_led_yellow(ON);
  } else {
    set_led_yellow(OFF);
  }
}

void handle_task_red_led() {
  uint32_t current_cycle = system_uptime() / 100;
  if (current_cycle > last_red_cycle) {
    last_red_cycle = current_cycle;
    uint8_t redval = PINB & _BV(PB4);
    if (redval == 0) {
      set_led_red(ON);
    } else {
      set_led_red(OFF);
    }
  }
}

void handle_task_green_led_count() {
  green_led_count++;
}

bool one_in_four() {
   int random_int = (rand() % 4) + 1;
   if (random_int == 4) {
     return true;
   } else {
     return false;
   }
}

void handle_task_jitter_led() {
  if (one_in_four() == true) {
    set_on_board_yellow(ON);
    _delay_ms(5);
    set_on_board_yellow(OFF);
  }
}

void flash_all_leds() {
  init_leds();
  set_led_green(ON);
  set_led_yellow(ON);
  set_led_red(ON);
  set_on_board_red(ON);
  set_on_board_green(ON);
  set_on_board_yellow(ON);
  _delay_ms(1000);
  set_led_green(OFF);
  set_led_yellow(OFF);
  set_led_red(OFF);
  set_on_board_red(OFF);
  set_on_board_green(OFF);
  set_on_board_yellow(OFF);
}

/************************************************
  Setup/Init
*************************************************/
void init_serial() {
  SetupHardware();
  sei();
}

void init_leds() {
  init_on_board_leds();
  DDRD |= (1 << DDD6);
  DDRB |= (1 << DDB6);
  DDRB |= (1 << DDB4);
}

void init_jitter_led_task() {
  srand(0);
}

void init_task_green_led() {
  TCCR1A |= (1 << COM1B1) | (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << CS12) | (1 << CS10);
  ICR1 = 1562;
  OCR1B = ICR1 / 2;
  TIMSK1 = (0 << ICIE1) | (0 << OCIE1B) | (0 << OCIE1A) | (1 << TOIE1);
}

void init_task_yellow_led() {
  TCCR3A = 0x00;
  TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
  OCR3A = 6250;
  TIMSK3 = (1 << OCIE3A);
}

void init_all_tasks() {
  init_leds();
  init_1000hz_timer_0();
  init_task_green_led();
  init_task_yellow_led();
  init_jitter_led_task();
  init_input_buttons();
  sei();
}

/************************************************
  Experiments
*************************************************/

void setup_experiment() {
}

void run_experiment_1() {
  printf("Running experiment 1...\r\n");
  // TODO: Implement me
  printf("Experiment 1 complete.\r\n");
}

void run_experiment_2() {
  printf("Running experiment 2...\r\n");
  // TODO: Implement me
  printf("Experiment 2 complete.\r\n");
}

void run_experiment_3() {
  printf("Running experiment 3...\r\n");
  // TODO: Implement me
  printf("Experiment 3 complete.\r\n");
}

void run_experiment_4() {
  printf("Running experiment 4...\r\n");
  // TODO: Implement me
  printf("Experiment 4 complete.\r\n");
}

void run_experiment_5() {
  printf("Running experiment 5...\r\n");
  // TODO: Implement me
  printf("Experiment 5 complete.\r\n");
}

void run_experiment_6() {
  printf("Running experiment 6...\r\n");
  // TODO: Implement me
  printf("Experiment 6 complete.\r\n");
}

void run_experiment_7() {
  printf("Running experiment 7...\r\n");
  // TODO: Implement me
  printf("Experiment 7 complete.\r\n");
}

void run_experiment_8() {
  printf("Running experiment 8...\r\n");
  // TODO: Implement me
  printf("Experiment 8 complete.\r\n");
}

/************************************************
  Main Program
*************************************************/
void loop_menu() {
  if (menu_first == false) {
    menu_first = true;
    normal_first = false;
    printf("Initializing menu...\r\n");
    halt();
    print_usage();
  }

  handle_menu();
}

void loop_tasks() {
  if (normal_first == false) {
    normal_first = true;
    menu_first = false;
    printf("Resuming program execution...\r\n");
    reset_state();
    init_all_tasks();
  }

  handle_task_red_led();

  if (jitter_release == true) {
    handle_task_jitter_led();
    jitter_release = false;
  }
}

int main() {
  flash_all_leds();
  init_serial();
  initial_prompt();

  while (1) {
    USB_Mainloop_Handler();

    if (menu_release == true) {
      loop_menu();
    } else {
      loop_tasks();
    }
  }
}
