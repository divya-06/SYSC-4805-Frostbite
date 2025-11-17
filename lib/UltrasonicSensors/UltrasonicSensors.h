#ifndef ULTRASONIC_SENSORS_H
#define ULTRASONIC_SENSORS_H

#include <Arduino.h>   // gives millis, pulseIn, digitalWrite, etc.

void initUltrasonic();
void updateUltrasonic();
bool isObstacleStable();
float getDist1();
float getDist2();

#endif
