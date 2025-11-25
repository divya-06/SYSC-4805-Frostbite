#include "UltrasonicSensors.h"

 
// Sensor Pins
#define TRIG1 7
#define ECHO1 6
#define TRIG2 5
#define ECHO2 4
#define TRIG3 3
#define ECHO3 2

 
// Distance Storage
static float dist1 = 0.0f;
static float dist2 = 0.0f;
static float dist3 = 0.0f;

 
// Obstacle Detection
const float  OBSTACLE_THRESHOLD_CM = 25.0f;
const uint8_t OB_DETECT_COUNT_REQ  = 2;
static uint8_t obDetectCount = 0;

 
// Distance Helper
static float getDistanceCM(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 50000);   // 50ms timeout
  float distance = duration * 0.0343f / 2.0f;      // cm
  return distance;
}

 
// Setup
void initUltrasonic() {
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  pinMode(TRIG3, OUTPUT);
  pinMode(ECHO3, INPUT);
}

 
// Update Readings
void updateUltrasonic() {
  static unsigned long lastUS = 0;

  if (millis() - lastUS >= 10) {   // ~20Hz
    lastUS = millis();

    dist1 = getDistanceCM(TRIG1, ECHO1);
    dist2 = getDistanceCM(TRIG2, ECHO2);
    dist3 = getDistanceCM(TRIG3, ECHO3);

    bool obstacleNow =
      (dist1 > 0 && dist1 <= OBSTACLE_THRESHOLD_CM) ||
      (dist2 > 0 && dist2 <= OBSTACLE_THRESHOLD_CM) ||
      (dist3 > 0 && dist3 <= OBSTACLE_THRESHOLD_CM);

    if (obstacleNow) {
      if (obDetectCount < 255) obDetectCount++;
    } else {
      if (obDetectCount > 0) obDetectCount--;
    }
  }
}

 
// Stable Detection
bool isObstacleStable() {
  return (obDetectCount >= OB_DETECT_COUNT_REQ);
}

 
// Getters
float getDist1() { return dist1; }
float getDist2() { return dist2; }
