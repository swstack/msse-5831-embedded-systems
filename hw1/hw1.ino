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

int stateButtonA = 0;
int stateButtonC = 0;

void setup()
{

}

void loop()
{
  if (buttonA.getSingleDebouncedRelease()) {
    if (stateButtonA < 2) {
      stateButtonA++;
    } else {
      stateButtonA = 0;
    }
  }

  if (buttonC.getSingleDebouncedRelease()) {
    if (stateButtonC < 2) {
      stateButtonC++;
    } else {
      stateButtonC = 0;
    }
  }

  updateGreenLEDState(stateButtonA);
  updateYellowLEDState(stateButtonC);
}

void updateGreenLEDState(int stateButtonA) {
  Serial.println(stateButtonA);
  switch(stateButtonA) {

    case 1:  // solid on
      ledGreen(1);
      break;

    case 2:  // blink at 2hz
      ledGreen(1);
      delay(200);
      ledGreen(0);
      break;

    default:  // off
      ledGreen(0);
  }
}

void updateYellowLEDState(int stateButtonC) {
  switch(stateButtonC) {

    case 1:  // solid on
      ledYellow(1);
      break;

    case 2:  // blink at 2hz
      ledYellow(1);
      delay(200);
      ledYellow(0);
      break;

    default:  // off
      ledYellow(0);
  }
}
