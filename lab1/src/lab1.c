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
void reset_task_state();
void reset_experiment_state();
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
int normal_first = false;
bool menu_first = false;

// Task state

int task_missed_count_red_led = 0;
int task_missed_count_yellow_led = 0;
int task_missed_count_jitter_led = 0;
int task_missed_count_hough_transform = 0;
int task_missed_count_green_led_count = 0;

int task_complete_count_red_led = 0;
int task_complete_count_yellow_led = 0;
int task_complete_count_jitter_led = 0;
int task_complete_count_hough_transform = 0;
int task_complete_count_green_led_count = 0;

bool task_release_red_led = false;
bool task_release_yellow_led = false;
bool task_release_jitter_led = false;
bool task_release_hough_transform = false;
bool task_release_green_led_count = false;

uint32_t red_led_ms_counter = 0;
uint8_t counter_10hz = 0;
uint8_t green_led_count = 0;

// Experiments state
bool experiment_1_running = false;
bool experiment_2_running = false;
bool experiment_3_running = false;
bool experiment_4_running = false;
bool experiment_5_running = false;
bool experiment_6_running = false;
bool experiment_7_running = false;
bool experiment_8_running = false;

uint32_t experiment_time_remaining = 0;

/************************************************
  ISRs
*************************************************/
ISR(PCINT0_vect) {
  // Interrutps when Button A is pushed

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
  // Interrupts every 25ms (40hz)

  counter_10hz++;

  if (counter_10hz >= 4) {
    counter_10hz = 0;

    // Enter this block every 100ms (10hz)

    // Handle yellow LED task release
    if (task_release_yellow_led == true) {
      // Task missed!!!
      task_missed_count_yellow_led++;
    } else {
      task_release_yellow_led = true;
    }

    // Handle hough transform task release
    if (task_release_hough_transform == true) {
      // Task missed!!!
      task_missed_count_hough_transform++;
    } else {
      task_release_hough_transform = true;
    }

    counter_10hz = 0;
  }

  // Handle jitter LED task release
  if (task_release_jitter_led == true) {
    // Task missed!!!
    task_missed_count_jitter_led++;
  } else {
    task_release_jitter_led = true;
  }

}

ISR(TIMER1_OVF_vect) {
  // PWM timer
  task_release_green_led_count = true;
}

ISR(TIMER0_COMPA_vect) {
  // Every 1000hz

  uptime_ms++;
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

void reset_task_state() {
  task_release_red_led = false;
  task_release_yellow_led = false;
  task_release_jitter_led = false;
  task_release_hough_transform = false;
  task_release_green_led_count = false;
  red_led_ms_counter = 0;
  counter_10hz = 0;
  green_led_count = 0;
  task_missed_count_red_led = 0;
  task_missed_count_yellow_led = 0;
  task_missed_count_jitter_led = 0;
  task_missed_count_hough_transform = 0;
  task_missed_count_green_led_count = 0;
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
  if (task_release_hough_transform == true) {
    volatile char dummyVar;
    dummyVar = houghTransform((uint16_t)&image_red, (uint16_t)&image_green, (uint16_t)&image_blue);
    task_complete_count_hough_transform++;
    task_release_hough_transform = false;
  }
}

void handle_task_yellow_led() {
  if (task_release_yellow_led == true) {
    uint8_t yelval = PIND & _BV(PD6);
    if (yelval == 0) {
      set_led_yellow(ON);
    } else {
      set_led_yellow(OFF);
    }

    if (experiment_3_running == true) {
      _delay_ms(20);
    }

    if (experiment_5_running == true) {
      _delay_ms(30);
    }

    if (experiment_7_running == true) {
      _delay_ms(105);
    }

    task_complete_count_yellow_led++;
    task_release_yellow_led = false;
  }

}

void handle_task_red_led() {

  if (system_uptime() > 100) {
    // Something took greater than 100ms...we missed our red task
    task_missed_count_red_led++;
    reset_uptime();
  }

  if (system_uptime() == 100) {
    reset_uptime();

    uint8_t redval = PINB & _BV(PB4);
    if (redval == 0) {
      set_led_red(ON);
    } else {
      set_led_red(OFF);
    }

    task_complete_count_red_led++;
  }
}

void handle_task_green_led_count() {
  if (task_release_green_led_count == true) {

    green_led_count++;
    task_complete_count_green_led_count = task_complete_count_green_led_count + 2;
    task_release_green_led_count = false;

    if (experiment_2_running == true) {
      _delay_ms(20);
    }

    if (experiment_4_running == true) {
      _delay_ms(30);
    }

    if (experiment_6_running == true) {
      _delay_ms(105);
    }

  }
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
  if (task_release_jitter_led == true) {
    if (one_in_four() == true) {
      set_on_board_yellow(ON);
      _delay_ms(5);
      set_on_board_yellow(OFF);
    }
    task_complete_count_jitter_led++;
    task_release_jitter_led = false;
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
  Command callbacks
*************************************************/
void command_print() {
  char *stats = "Stats:\r\n"
                "Green LED       : Completions (%i) Misses (%i)\r\n"
                "Yellow LED      : Completions (%i) Misses (%i)\r\n"
                "Red LED         : Completions (%i) Misses (%i)\r\n"
                "Jitter LED      : Completions (%i) Misses (%i)\r\n"
                "Hough Transform : Completions (%i) Misses (%i)\r\n";
  printf(
    stats,
    task_complete_count_green_led_count,  task_missed_count_green_led_count,
    task_complete_count_yellow_led,       task_missed_count_yellow_led,
    task_complete_count_red_led,          task_missed_count_red_led,
    task_complete_count_jitter_led,       task_missed_count_jitter_led,
    task_complete_count_hough_transform,  task_missed_count_hough_transform
  );
}

/************************************************
  Main Program
*************************************************/
void loop_menu() {
  if (menu_first == false) {
    menu_first = true;
    normal_first = false;
    halt();
    print_usage();
  }

  USB_Mainloop_Handler();
  handle_menu();
}

void loop_tasks() {
  if (normal_first == false) {
    normal_first = true;
    menu_first = false;
    reset_task_state();
    init_all_tasks();
  }

  handle_task_red_led();
  handle_task_jitter_led();
  handle_task_hough_transform();
  handle_task_yellow_led();
  handle_task_green_led_count();
}

int main() {

  on_command_print = command_print;

  flash_all_leds();
  init_serial();
  initial_prompt();

  // Run an experiment by uncommenting
  // experiment_1_running = true;
  // experiment_2_running = true;
  // experiment_3_running = true;
  experiment_4_running = true;
  // experiment_5_running = true;
  // experiment_6_running = true;
  // experiment_7_running = true;
  // experiment_8_running = true;

  while (1) {

    if (menu_release == true) {
      loop_menu();
    } else {
      loop_tasks();
    }
  }
}
