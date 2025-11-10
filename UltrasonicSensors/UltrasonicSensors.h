// UltrasonicSensors.h
#pragma once
#include <Arduino.h>

// Call in setup()
void initUltrasonic();

// Call regularly in loop()
void updateUltrasonic();

// Get latest distances (cm)
float getDist1();
float getDist2();

// Stable obstacle detection flag
bool isObstacleStable();
