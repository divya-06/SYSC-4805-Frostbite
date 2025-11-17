// MotorControl.h
#pragma once
#include <Arduino.h>
#include "CytronMotorDriver.h"

void initMotors();

void moveForward(int v);
void moveBackward(int v);
void turnCW(int v);
void stopCar();
