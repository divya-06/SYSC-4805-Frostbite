#include "IRObstacleSensor.h"

// === Pin configuration ===
// Change this if you wire OUT to another digital pin.
static const uint8_t IR_SENSOR_PIN = 45;

// How many consecutive "obstacle" readings we require
// before we consider it stable (debounce).
static const uint8_t IR_DETECT_COUNT_REQ = 3;

static uint8_t irDetectCount = 0;
static bool    irObstacleNow = false;

void initIrObstacleSensor() {
  // VMA330 has its own output driver, usually with internal pull-up.
  pinMode(IR_SENSOR_PIN, INPUT);
}

void updateIrObstacleSensor() {
  int raw = digitalRead(IR_SENSOR_PIN);

  // Lab manual: reading 1 = no obstacles, 0 = obstacle
  // So we invert it here so true means "obstacle seen".
  bool obstacle = (raw == LOW);

  irObstacleNow = obstacle;

  // Simple up/down counter to smooth noise
  if (obstacle) {
    if (irDetectCount < 255) irDetectCount++;
  } else {
    if (irDetectCount > 0) irDetectCount--;
  }
}

bool isIrObstacleNow() {
  return irObstacleNow;
}

bool isIrObstacleStable() {
  return (irDetectCount >= IR_DETECT_COUNT_REQ);
}
