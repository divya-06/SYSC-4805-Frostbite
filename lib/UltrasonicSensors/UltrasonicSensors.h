#pragma once
#include <Arduino.h>

 
// Setup
void initUltrasonic();

 
// Update
void updateUltrasonic();

 
// Distance Values
float getDist1();
float getDist2();

 
// Stable Detection
bool isObstacleStable();
