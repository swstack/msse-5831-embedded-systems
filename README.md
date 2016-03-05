# MSSE: SENG-5831 Class Assignments

Class assignments for SENG-5831 (Embedded Systems) at the University of Minnesota.

# Pins

Button A
  Pin = PB3
  Interrupt = PCINT3

Button B
  Pin = PD5

Button C
  Pin = PB0
  Interrupt = PCINT0

# HW 1

Push buttons to modify LED functionality.

__Button A Requirements:__

* When Button A is pushed once, turn the green LED on.
* When Button A is pushed twice, blink green LED at 2 Hz (every 250ms)
* When Button A is pushed a third time, turn the green LED off.

__Button C Requirements:__

* When Button C is pushed once, turn the yellow LED on.
* When Button C is pushed twice, blink yellow LED at 2 Hz (every 1250ms)
* When Button C is pushed a third time, turn the yellow LED off.

# HW 2

Intro to ISRs.

* Blink the Green on-board LED () using the main loop at a rate of .5 Hz.

* ISR based on button A. If button A is pushed and released, then blink a different LED (general I/O) 10 times on/off with an empty for-loop for delay (time it for about 4 Hz). This happens inside the ISR. NOTE: ISR will fire on both press and release of button. The LED should blink only after the release of the button.

* Create rudimentary menu that changes which color is blinking (or port) at 4 Hz. Currently there is no terminal-based UI capability, thus you can use button presses as a means to change LEDs (I will leave it as your choice to change what you like - color, frequency, or maybe an interesting pattern of blinking LEDs.)

# HW 3

Create a single program that contains the following periodic tasks:

* A timer that increments a global variable time_ms, which records the number of ms the system has been running. This timer should be implemented with a timer/counter interrupt at a frequency of 1000 Hz. Things to think about: the variable type controls how long the system can stay live until time_ms overflows. You do not want to be interrupted while reading the timer in the main loop.

* Toggle an LED on the board at a frequency of 1 Hz. This task should be implemented inside an ISR for a timer/counter set to the frequency of the toggle. This should be a separate timer/counter from that used to maintain time_ms.

* Toggle an LED on the board at a frequency of 4 Hz. This task should be implemented in the main loop. Set a timer to expire based on the global variable time_ms to determine when to toggle the LED. You should attempt to keep this toggling at the same rate even when the computationally intensive task is running.

* Poll for a button press at a “reasonable” rate. If you see the button press (i.e. pin reads high), add in a very brief delay (i.e. a blind cycle) for the release of the button, then release the computationally intensive task. A release is performed by setting a global flag that is checked in the main loop. The periodic polling should be implemented inside an ISR - you can use either of the timer/counters set up for the other tasks.

* A computationally intense pseudo task, that is executed inside the main loop. It is released by the polled event of the button press (ignore release of button). It contains these steps:

  1. Turn on a red LED “plugged into” general I/O (see note below), then spin in a for-loop for 300 ms.
  2. Turn off the red LED from (1), and turn on a green (or yellow) LED also in I/O, then spin in a for-loop for 500 ms.
  3. Turn off the green/yellow, turn the red back on, then spin in a for-loop for 400 ms.
  4. Turn off the green/yellow red LED. (Had a typo on the LED color!)
