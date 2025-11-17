// Encoders.h
#pragma once
#include <Arduino.h>

void initEncoders();

long getRightTicks();
long getLeftTicks();

void resetTicks();
