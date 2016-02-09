# MSSE: SENG-5831 Class Assignments

Class assignments for SENG-5831 (Embedded Systems) at the University of Minnesota.

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

ISR based on button A. If button A is pushed and released, then blink a different LED (general I/O) 10 times on/off with an empty for-loop for delay (time it for about 4 Hz). This happens inside the ISR. NOTE: ISR will fire on both press and release of button. The LED should blink only after the release of the button.

Create rudimentary menu that changes which color is blinking (or port) at 4 Hz. Currently there is no terminal-based UI capability, thus you can use button presses as a means to change LEDs (I will leave it as your choice to change what you like - color, frequency, or maybe an interesting pattern of blinking LEDs.)
