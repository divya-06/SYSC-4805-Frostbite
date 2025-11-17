// Encoders.cpp
#include "Encoders.h"

const uint8_t ENC_RIGHT = A2;
const uint8_t ENC_LEFT  = A3;

static volatile long rightTicks = 0;
static volatile long leftTicks  = 0;

// ISR handlers
void isrRight() { rightTicks++; }
void isrLeft()  { leftTicks++; }

// Hardware setup
void initEncoders() {
  #ifdef ARDUINO
      pinMode(ENC_RIGHT, INPUT_PULLUP);
      pinMode(ENC_LEFT,  INPUT_PULLUP);

      attachInterrupt(digitalPinToInterrupt(ENC_RIGHT), isrRight, RISING);
      attachInterrupt(digitalPinToInterrupt(ENC_LEFT),  isrLeft,  RISING);
  #endif
}

// Safe getters
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
