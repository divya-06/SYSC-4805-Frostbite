#include <Arduino.h>
#include "MotorControl/MotorControl.h"
#include "LineSensors/LineSensors.h"
#include "UltrasonicSensors/UltrasonicSensors.h"
#include "Encoders/Encoders.h"
#include "WatchdogTimer/WatchdogTimer.h"

// =================== FSM ===================
enum State {
  FORWARD,
  STOP1_AFTER_LINE,
  REVERSE,
  STOP2_BEFORE_TURN,
  TURN_CLOCKWISE,
  STOP3_AFTER_TURN
};

State state = FORWARD;
unsigned long stateStart = 0;

void enterState(State s) {
  state = s;
  stateStart = millis();
}

// ------------- Parameters --------------
int  forwardSpeed = 140;
int  reverseSpeed = 140;
int  turnSpeed = 150;
unsigned long reverseTimeMs = 500; // 0.5 s reverse
unsigned long settleStopMs = 150; // small pause
unsigned long turnTimeMs = 1500; // ~90° (tune)

// ---------------- Setup ----------------
void setup() {
  Serial.begin(115200);

  initMotors();
  initLineSensors();
  initUltrasonic();
  initEncoders();
  initWatchdog(10000);   // initialize watchdog timer (10s timeout)

  setLineThreshold(1004);

  Serial.println("Forward → detect line OR obstacle ≤20cm → stop → reverse 0.5s → stop → 90° CW turn → stop → repeat");
}

// ---------------- Loop ----------------
void loop() {
  // Update sensors
  updateLineSensors();
  updateUltrasonic();

  resetWatchdog(); // reset the watchdog each iteration

  bool lineDetectedStable = isLineDetectedStable();
  bool obstacleStable = isObstacleStable();
  uint16_t rawL = getRawL();
  uint16_t rawM = getRawM();
  uint16_t rawR = getRawR();
  uint16_t avgL = getAvgL();
  uint16_t avgM = getAvgM();
  uint16_t avgR = getAvgR();
  float d1 = getDist1();
  float d2 = getDist2();
  long rTicks = getRightTicks();
  long lTicks = getLeftTicks();

  // --- Debug print every 200 ms ---
  static unsigned long prevPrint = 0;
  if (millis() - prevPrint >= 200) {
    prevPrint = millis();
    Serial.print("RAW LMR: "); Serial.print(rawL); Serial.print(" ");
    Serial.print(rawM); Serial.print(" "); Serial.println(rawR);

    Serial.print("AVG LMR: "); Serial.print(avgL); Serial.print(" ");
    Serial.print(avgM); Serial.print(" "); Serial.println(avgR);

    Serial.print("US cm: d1="); Serial.print(d1, 1);
    Serial.print(" d2="); Serial.print(d2, 1);

    Serial.print(" | lineDetStable="); Serial.print(lineDetectedStable);
    Serial.print(" | obStable="); Serial.print(obstacleStable);

    Serial.print(" | Ticks R/L: "); Serial.print(rTicks);
    Serial.print("/"); Serial.println(lTicks);
  }

  // --- FSM ---
  switch (state) {
    case FORWARD:
      moveForward(forwardSpeed);
      // Trigger on line OR obstacle
      if (lineDetectedStable || obstacleStable) {
        stopCar();
        if (obstacleStable) Serial.println("Obstacle ≤20cm → STOP1");
        else Serial.println("Line detected → STOP1");
        enterState(STOP1_AFTER_LINE);
      }
      break;

    case STOP1_AFTER_LINE:
      stopCar();
      if (millis() - stateStart >= settleStopMs) {
        Serial.println("Reverse 0.5s");
        enterState(REVERSE);
      }
      break;

    case REVERSE:
      moveBackward(reverseSpeed);
      if (millis() - stateStart >= reverseTimeMs) {
        stopCar();
        Serial.println("Reverse done → STOP2");
        enterState(STOP2_BEFORE_TURN);
      }
      break;

    case STOP2_BEFORE_TURN:
      stopCar();
      if (millis() - stateStart >= settleStopMs) {
        Serial.println("Turning CW ~90°");
        resetTicks();
        enterState(TURN_CLOCKWISE);
      }
      break;

    case TURN_CLOCKWISE:
      turnCW(turnSpeed);
      if (millis() - stateStart >= turnTimeMs) {
        stopCar();
        Serial.println("Turn complete → STOP3");
        enterState(STOP3_AFTER_TURN);
      }
      break;

    case STOP3_AFTER_TURN:
      stopCar();
      if (millis() - stateStart >= settleStopMs) {
        Serial.println("Repeat cycle → FORWARD");
        enterState(FORWARD);
      }
      break;
  }

  delay(20); // small loop delay for stability
}
