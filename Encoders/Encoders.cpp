// Encoders.cpp
#include "Encoders.h"

const uint8_t ENC_RIGHT = A2;  // right-rear encoder (motor 2)
const uint8_t ENC_LEFT  = A3;  // left-rear  encoder (motor 3)

static volatile long rightTicks = 0;
static volatile long leftTicks  = 0;

void isrRight() { rightTicks++; }
void isrLeft()  { leftTicks++;  }

void initEncoders() {
  pinMode(ENC_RIGHT, INPUT_PULLUP);
  pinMode(ENC_LEFT,  INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_RIGHT), isrRight, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_LEFT),  isrLeft,  RISING);
}

long getRightTicks() {
  noInterrupts();
  long r = rightTicks;
  interrupts();
  return r;
}

long getLeftTicks() {
  noInterrupts();
  long l = leftTicks;
  interrupts();
  return l;
}

void resetTicks() {
  noInterrupts();
  rightTicks = 0;
  leftTicks  = 0;
  interrupts();
}
