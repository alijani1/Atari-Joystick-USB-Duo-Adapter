//-------------------------------------------------------------------- 
//
// Atari USB Joystick Adapter with dual joystick support
// Code Developed by Ali Jani @ iCode
//
//--------------------------------------------------------------------

#define UP     0
#define DOWN   1
#define LEFT   2
#define RIGHT  3
#define FIRE   4
#define REWIND 5
#define SELECT 6
#define START  7
#define MENU   8
#define NONE   99

#include <Joystick.h>
#include <Wire.h>

#define JOYSTICK_COUNT 2

Joystick_ Joystick[JOYSTICK_COUNT] = {
  Joystick_(0x03, JOYSTICK_TYPE_GAMEPAD,
  5, 0,                  // Button Count, Hat Switch Count
  true, true, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false),  // No accelerator, brake, or steering
  Joystick_(0x04, JOYSTICK_TYPE_GAMEPAD,
  5, 0,                  // Button Count, Hat Switch Count
  true, true, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false)  // No accelerator, brake, or steering
};

// Last state of the buttons
int lastButtonState[JOYSTICK_COUNT][5] = {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
int cButtonsState[5] = {0, 0, 0, 0, 0};

// pins order are {UP,Down,Left,Right,Fire}
// Ver1
int LeoPins[JOYSTICK_COUNT][5] {{2,3,4,5,6},{7,8,9,10,15}};

// Ver2: if you dont want to use pins 2 and 3
// int LeoPins[JOYSTICK_COUNT][5] {{14, 16, 4, 5, 6}, {7, 8, 9, 10, 15}};

int SpecialButtonState = NONE;
bool changed = false;

void setup() {

  Wire.begin();
  
  // Initialize input Pins
  for (int j=0; j< JOYSTICK_COUNT; j++) {
    for (int p=0; p<5; p++) {
      pinMode( LeoPins[j][p], INPUT_PULLUP);
    }
  }
    
  // Initialize Joystick Library
  for (int index = 0; index < JOYSTICK_COUNT; index++)
  {
    Joystick[index].begin();
    Joystick[index].setXAxisRange(-1, 1);
    Joystick[index].setYAxisRange(-1, 1);
  }
}

void loop() {
  for (int js = 0; js < JOYSTICK_COUNT; js++)
  {
    changed = false;
    for (int index = 0; index < 5; index++)
    {
      cButtonsState[index] = !digitalRead(LeoPins[js][index]);
      if  (cButtonsState[index] != lastButtonState[js][index]) {
        changed = true;
      }
      lastButtonState[js][index] = cButtonsState[index];
    }

    if (changed) {
      /* Handle special buttons on 5 button joysticks
         REWIND = LEFT + RIGHT + UP
         SELECT = LEFT + RIGHT
         START = UP + DOWN + LEFT
         MENU = UP + DOWN
      */
      SpecialButtonState = NONE;
      if ((cButtonsState[LEFT] == 1) && (cButtonsState[RIGHT] == 1) && (cButtonsState[UP] == 1)) {
        SpecialButtonState = REWIND;
      } else if ((cButtonsState[LEFT] == 1) && (cButtonsState[RIGHT] == 1) ) {
        SpecialButtonState = SELECT;
      } else if ((cButtonsState[UP] == 1) && (cButtonsState[DOWN] == 1) && (cButtonsState[LEFT] == 1)) {
        SpecialButtonState = START;
      } else if ((cButtonsState[UP] == 1) && (cButtonsState[DOWN] == 1)) {
        SpecialButtonState = MENU;
      }
      Joystick[js].setButton(4, (SpecialButtonState == REWIND) ? 1 : 0);
      Joystick[js].setButton(2, (SpecialButtonState == SELECT) ? 1 : 0);
      Joystick[js].setButton(3, (SpecialButtonState == START) ? 1 : 0);
      Joystick[js].setButton(1, (SpecialButtonState == MENU) ? 1 : 0);

      // handle directions and Fire
      if (SpecialButtonState == NONE) {
        if (cButtonsState[UP] == 1) {
          Joystick[js].setYAxis(-1);
        } else if (cButtonsState[DOWN] == 1) {
          Joystick[js].setYAxis(1);
        } else {
          Joystick[js].setYAxis(0);
        }

        if (cButtonsState[LEFT] == 1) {
          Joystick[js].setXAxis(-1);
        } else if (cButtonsState[RIGHT] == 1) {
          Joystick[js].setXAxis(1);
        } else {
          Joystick[js].setXAxis(0);
        }

        if (cButtonsState[FIRE] == 1) {
          Joystick[js].setButton(0, 1);
        } else {
          Joystick[js].setButton(0, 0);
        }
      }
    }
    delay(5);
  }
}