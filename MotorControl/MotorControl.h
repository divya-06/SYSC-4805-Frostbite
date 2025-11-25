#pragma once
#include <Arduino.h>
#include "CytronMotorDriver.h"

void initMotors();

 
// Movement
void moveForward(int v);
void moveBackward(int v);
void turnCW(int v);
void turnCCW(int v);
void stopCar();
