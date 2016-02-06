#include <AStar32U4.h>

AStar32U4ButtonA buttonA;
AStar32U4ButtonB buttonB;
AStar32U4ButtonC buttonC;

// Button State
int stateButtonA = 0;
int stateButtonC = 0;

// LED Blink state
int ledBlinkCounterGreen = 0;
int tmpCounterGreen;
int ledBlinkCounterYellow = 0;
int tmpCounterYellow;
unsigned long uptime;

// Use #defines or const to define states and improve readability. (ACL)

// LED on/off state
int greenLEDState;
int yellowLEDState;

// I think this grouping of state and behavior in one spot is a good thing. (ACL)
void _ledGreen(int state) {
  greenLEDState = state;
  ledGreen(state);
}

void _ledYellow(int state) {
  yellowLEDState = state;
  ledYellow(state);
}

void setup()
{

}

void loop()
{
  // Can use mod to advance state without checking current. (ACL)
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

  switch(stateButtonA) {

    case 1:  // solid on
      _ledGreen(1);
      break;

    case 2:  // blink at 2hz
      handleGreenBlink();
      break;

    default:  // off
      _ledGreen(0);
  }
}

void updateYellowLEDState(int stateButtonC) {

  switch(stateButtonC) {

    case 1:  // solid on
      _ledYellow(1);
      break;

    case 2:  // blink at .4hz
      handleYellowBlink();
      break;

    default:  // off
      _ledYellow(0);
  }
}

void handleGreenBlink() {
  uptime = millis();
  tmpCounterGreen = uptime / 250;
  if (tmpCounterGreen > ledBlinkCounterGreen) {
    ledBlinkCounterGreen = tmpCounterGreen;

    if (greenLEDState == 0) {
      _ledGreen(1);
    } else {
      _ledGreen(0);
    }
  }
}

// A logically correct implementation with good control over timing, but difficult to follow. (ACL)
void handleYellowBlink() {
  uptime = millis();
  tmpCounterYellow = uptime / 1250;
  if (tmpCounterYellow > ledBlinkCounterYellow) {
    ledBlinkCounterYellow = tmpCounterYellow;

    if (yellowLEDState == 0) {
      _ledYellow(1);
    } else {
      _ledYellow(0);
    }
  }
}
