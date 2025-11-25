// MotorControl.h
#pragma once
#include <Arduino.h>
#include "CytronMotorDriver.h"

// Initialize motors
void initMotors();

// Movement helpers
void moveForward(int v);
void moveBackward(int v);
void turnCW(int v);
void turnCCW(int v);
void stopCar();
