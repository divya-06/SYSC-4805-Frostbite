// UltrasonicSensors.cpp
#include "UltrasonicSensors.h"

// Pins
const uint8_t TRIG1 = 7;
const uint8_t ECHO1 = 6;
const uint8_t TRIG2 = 4;
const uint8_t ECHO2 = 3;

static float dist1 = 0.0f;
static float dist2 = 0.0f;

// Obstacle detection
static const float  OBSTACLE_THRESHOLD_CM = 15.0f;
static const uint8_t OB_DETECT_COUNT_REQ  = 2;
static uint8_t obDetectCount = 0;

static float getDistanceCM(uint8_t trigPin, uint8_t echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 50000);  // 50 ms timeout
    return duration * 0.0343f / 2.0f;               // cm
}

void initUltrasonic() {
#ifdef ARDUINO
    pinMode(TRIG1, OUTPUT);
    pinMode(ECHO1, INPUT);
    pinMode(TRIG2, OUTPUT);
    pinMode(ECHO2, INPUT);
#endif
}

void updateUltrasonic() {
    static unsigned long lastUS = 0;
    bool obstacleNow = false;

#ifdef ARDUINO
    if (millis() - lastUS < 50) return;
    lastUS = millis();

    dist1 = getDistanceCM(TRIG1, ECHO1);
    dist2 = getDistanceCM(TRIG2, ECHO2);

    if ((dist1 > 0 && dist1 <= OBSTACLE_THRESHOLD_CM) ||
        (dist2 > 0 && dist2 <= OBSTACLE_THRESHOLD_CM)) {
        obstacleNow = true;
    }
#else
    // Native test path
    dist1 = 10.0f;
    dist2 = 10.0f;
    obstacleNow = true;
#endif

    if (obstacleNow) {
        if (obDetectCount < 255) ++obDetectCount;
    } else {
        if (obDetectCount > 0) --obDetectCount;
    }
}

bool isObstacleStable() {
    return obDetectCount >= OB_DETECT_COUNT_REQ;
}

float getDist1() { return dist1; }
float getDist2() { return dist2; }
