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
  #ifdef ARDUINO
      pinMode(ENC_RIGHT, INPUT_PULLUP);
      pinMode(ENC_LEFT,  INPUT_PULLUP);

      attachInterrupt(digitalPinToInterrupt(ENC_RIGHT), isrRight, RISING);
      attachInterrupt(digitalPinToInterrupt(ENC_LEFT),  isrLeft,  RISING);
  #endif
}

 
// Get Right Ticks 
long getRightTicks() {
  #ifdef ARDUINO
    noInterrupts();
  #endif
      long val = rightTicks;
  #ifdef ARDUINO
      interrupts();
  #endif
      return val;
}


// Get Left Ticks
long getLeftTicks() {
  #ifdef ARDUINO
      noInterrupts();
  #endif
      long val = leftTicks;
  #ifdef ARDUINO
      interrupts();
  #endif
      return val;
}

 
// Reset Both Ticks
// Reset counters
void resetTicks() {
  #ifdef ARDUINO
      noInterrupts();
  #endif
      rightTicks = 0;
      leftTicks  = 0;
  #ifdef ARDUINO
      interrupts();
  #endif
}
