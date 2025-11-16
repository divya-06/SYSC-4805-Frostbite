#ifndef IR_OBSTACLE_SENSOR_H
#define IR_OBSTACLE_SENSOR_H

#include <Arduino.h>

// Initialize VMA330 (IR obstacle sensor)
void initIrObstacleSensor();

// Read the pin and update internal debounced state
void updateIrObstacleSensor();

// Instant reading: true = obstacle detected *right now*
bool isIrObstacleNow();

// Debounced (stable) reading: true = obstacle has been detected
// consistently for a few cycles (filters noise)
bool isIrObstacleStable();

#endif
