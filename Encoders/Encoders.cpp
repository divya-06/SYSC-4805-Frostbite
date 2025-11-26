#include "Encoders.h"

 
// Encoder Pins
const uint8_t ENC_RIGHT = A2;
const uint8_t ENC_LEFT  = A3;

 
// Tick Counters
static volatile long rightTicks = 0;
static volatile long leftTicks  = 0;

 
// ISR Handlers detects rising edge pulse
void isrRight() { rightTicks++; }
void isrLeft()  { leftTicks++;  }

 
// Encoder Setup
void initEncoders() {
  pinMode(ENC_RIGHT, INPUT_PULLUP); // pull up resistors 
  pinMode(ENC_LEFT,  INPUT_PULLUP);

 //ISR on rising edge (1 tick)
  attachInterrupt(digitalPinToInterrupt(ENC_RIGHT), isrRight, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_LEFT),  isrLeft,  RISING);
}

 
// Get Right Ticks 
long getRightTicks() {
  noInterrupts(); // pause tick count
  long r = rightTicks; // update current value
  interrupts(); // resume tick count
  return r;
}

 
// Get Left Ticks
long getLeftTicks() {
  noInterrupts();
  long l = leftTicks;
  interrupts();
  return l;
}

 
// Reset Both Ticks
void resetTicks() {
  noInterrupts(); // prevents other ISR to safely update values
  rightTicks = 0; // reset right tick
  leftTicks  = 0; // reset left tick
  interrupts(); 
}
