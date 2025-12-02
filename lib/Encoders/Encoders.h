#pragma once
#include <Arduino.h>

 
// Setup
void initEncoders();

 
// Tick Access
long getRightTicks();
long getLeftTicks();

 
// Reset
void resetTicks();
