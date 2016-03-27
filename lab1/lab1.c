#include <avr/io.h>
#include <common.h>
#include <menu.h>
#include <VirtualSerial.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <hough.h>

/************************************************
  Function Prototypes
*************************************************/
void reset_state();
void print_uptime();
void handle_task_yellow_led();
void handle_task_red_led();

/************************************************
  Flags/Globals
*************************************************/
bool menu_release = false;
int normal_first = false;
bool menu_first = false;
uint32_t last_red_cycle = 0;
uint8_t counter_40hz = 0;

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
    // handle_task_hough_transform();
    counter_40hz = 0;
  } else {
    counter_40hz++;
  }

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
  last_red_cycle = 0;
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
  printf("Executing Hough Transform...");
  volatile char dummyVar;
  uint32_t start = system_uptime();
  dummyVar = houghTransform((uint16_t)&red, (uint16_t)&green, (uint16_t)&blue);
  uint32_t end = system_uptime();
  printf("Hough Transform completed in %lu milliseconds.", end - start);
}

void handle_task_yellow_led() {
  uint8_t yelval = PIND & _BV(PD6);
  if (yelval == 0) {
    PORTD |= (1 << PORTD6);  // Turn on
  } else {
    PORTD &= ~(1 << PORTD6); // Turn off
  }
}

void handle_task_red_led() {
  uint32_t current_cycle = system_uptime() / 100;
  if (current_cycle > last_red_cycle) {
    last_red_cycle = current_cycle;
    uint8_t redval = PINB & _BV(PB4);
    if (redval == 0) {
      PORTB |= (1 << PORTB4);  // Turn on
    } else {
      PORTB &= ~(1 << PORTB4); // Turn off
    }
  }
}

void flash_all_leds() {
  init_leds();
  init_on_board_leds();
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
}

void init_leds() {
  DDRD |= (1 << DDD6);
  DDRB |= (1 << DDB6);
  DDRB |= (1 << DDB4);
}

void init_task_green_led() {
  TCCR1A |= (1 << COM1B1) | (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << CS12) | (1 << CS10);
  ICR1 = 1562;
  OCR1B = ICR1 / 2;
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
  init_input_buttons();
  sei();
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

  USB_Mainloop_Handler();
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
}

int main() {
  init_serial();

  flash_all_leds();
  // initial_prompt();

  while (1) {

    if (menu_release == true) {
      loop_menu();
    } else {
      loop_tasks();
    }
  }
}
