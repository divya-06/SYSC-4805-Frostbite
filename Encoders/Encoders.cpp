#include "Encoders.h"

 
// Encoder Pins
const uint8_t ENC_RIGHT = A2;
const uint8_t ENC_LEFT  = A3;

 
// Tick Counters
static volatile long rightTicks = 0;
static volatile long leftTicks  = 0;

 
// ISR Handlers
void isrRight() { rightTicks++; }
void isrLeft()  { leftTicks++;  }

 
// Encoder Setup
void initEncoders() {
  pinMode(ENC_RIGHT, INPUT_PULLUP);
  pinMode(ENC_LEFT,  INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_RIGHT), isrRight, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_LEFT),  isrLeft,  RISING);
}

 
// Get Right Ticks
long getRightTicks() {
  noInterrupts();
  long r = rightTicks;
  interrupts();
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
  noInterrupts();
  rightTicks = 0;
  leftTicks  = 0;
  interrupts();
}
