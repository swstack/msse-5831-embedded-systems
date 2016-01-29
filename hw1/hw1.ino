#include <AStar32U4.h>

AStar32U4ButtonA buttonA;
AStar32U4ButtonB buttonB;
AStar32U4ButtonC buttonC;

// Arbitrary LED identifiers
int GREEN_LED = 12;
int YELLOW_LED = 7;

// Button State
int stateButtonA = 0;
int stateButtonC = 0;

// LED Blink state
int ledBlinkCounter = 0;
int tmpCounter;
unsigned long uptime;

// LED on/off state
int greenLEDState;
int yellowLEDState;

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
      handleBlink(GREEN_LED);
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

    case 2:  // blink at 2hz
      handleBlink(YELLOW_LED);
      break;

    default:  // off
      _ledYellow(0);
  }
}

void handleBlink(int ledIdentifier) {
  uptime = millis();
  tmpCounter = uptime / 250;
  if (tmpCounter > ledBlinkCounter) {
    ledBlinkCounter = tmpCounter;

    if (ledIdentifier == GREEN_LED) {
      if (greenLEDState == 0) {
        _ledGreen(1);
      } else {
        _ledGreen(0);
      }
    }

    if (ledIdentifier == YELLOW_LED) {
      Serial.println(yellowLEDState);
      if (yellowLEDState == 0) {
        _ledYellow(1);
      } else {
        _ledYellow(0);
      }
    }
  }
}
