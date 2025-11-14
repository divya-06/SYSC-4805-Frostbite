// UltrasonicSensors.cpp
#include "UltrasonicSensors.h"

// Pins
#define TRIG1 7
#define ECHO1 6
#define TRIG2 4
#define ECHO2 3

static float dist1 = 0.0f;
static float dist2 = 0.0f;

// Obstacle detection params
const float OBSTACLE_THRESHOLD_CM = 15.0f;
const uint8_t OB_DETECT_COUNT_REQ = 2;
static uint8_t obDetectCount = 0;

static float getDistanceCM(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 50000); // 50 ms timeout
  float distance = duration * 0.0343f / 2.0f;    // cm
  return distance; // 0 if timeout
}

void initUltrasonic() {
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
}

void updateUltrasonic() {
  static unsigned long lastUS = 0;
  if (millis() - lastUS >= 50) { // ~20 Hz
    lastUS = millis();
    dist1 = getDistanceCM(TRIG1, ECHO1);
    dist2 = getDistanceCM(TRIG2, ECHO2);

    bool obstacleNow = false;
    if ((dist1 > 0 && dist1 <= OBSTACLE_THRESHOLD_CM) ||
        (dist2 > 0 && dist2 <= OBSTACLE_THRESHOLD_CM)) {
      obstacleNow = true;
    }

    if (obstacleNow) {
      if (obDetectCount < 255) obDetectCount++;
    } else {
      if (obDetectCount > 0) obDetectCount--;
    }
  }
}

bool isObstacleStable() {
  return (obDetectCount >= OB_DETECT_COUNT_REQ);
}

float getDist1() { return dist1; }
float getDist2() { return dist2; }
