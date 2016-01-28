/* This example demonstrates three different methods for
detecting a button press and release.  It blinks the green
user LED each time button A is pressed and released.

Note: This demo uses the standard button pins on the A-Star
32U4 Prime and A-Star 32U4 Robot Controller.  To avoid
damage or improper operation, if you have shields or other
electronics connected, make sure they do not use those pins
in a conflicting way. */

#include <AStar32U4.h>

// These objects provide access to the A-Star's on-board
// buttons.  We will only use buttonA.
AStar32U4ButtonA buttonA;
AStar32U4ButtonB buttonB;
AStar32U4ButtonC buttonC;

void setup()
{

}

void loop()
{
  while (1)
  {
    if (buttonA.getSingleDebouncedRelease())
    {
      // blink LED
      ledGreen(1);
      delay(200);
      ledGreen(0);
      break;
    }
  }
}
