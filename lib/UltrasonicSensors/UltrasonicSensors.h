#ifndef ULTRASONIC_SENSORS_H
#define ULTRASONIC_SENSORS_H

#include <Arduino.h>

void initUltrasonic();
void updateUltrasonic();

bool isObstacleStable();

float getDist1();
float getDist2();

#endif
