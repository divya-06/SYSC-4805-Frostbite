#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>

#include "MotorControl.h"
#include "LineSensors.h"
#include "UltrasonicSensors.h"
#include "Encoders.h"
#include "WatchdogTimer.h"

 
// Shared Sensor Variables 
volatile bool lineDetectedStable = false;
volatile bool obstacleStable     = false;

volatile uint16_t rawL, rawM, rawR;
volatile uint16_t avgL, avgM, avgR;
volatile float d1, d2;
volatile long rTicks, lTicks;

 
// Turn Direction Enum 
enum TurnDir {
  TURN_DIR_CW,
  TURN_DIR_CCW
};
volatile TurnDir turnDirection = TURN_DIR_CW;

 
// FSM States 
enum State {
  FORWARD,
  STOP1_AFTER_LINE,
  REVERSE,
  STOP2_BEFORE_TURN,
  TURNING,
  STOP3_AFTER_TURN
};

State state = FORWARD;
unsigned long stateStart = 0;

 
// State Entry Helper
void enterState(State s) {
  state = s;
  stateStart = millis();
}

 
// Movement Parameters 
int forwardSpeed  = 255;
int reverseSpeed  = 255;
int turnSpeed     = 255;

unsigned long reverseTimeMs = 500;
unsigned long settleStopMs  = 150;

const long TURN_TICKS_TARGET = 11; // ~135°

 
// Task Periods 
const TickType_t LINE_PERIOD_TICKS   = pdMS_TO_TICKS(5);
const TickType_t ULTRA_PERIOD_TICKS  = pdMS_TO_TICKS(10);
const TickType_t FSM_PERIOD_TICKS    = pdMS_TO_TICKS(5);
const TickType_t DEBUG_PERIOD_TICKS  = pdMS_TO_TICKS(200);

 
// Line Sensor Task 
void vTaskLineSensors(void *pv) {
  TickType_t lastWake = xTaskGetTickCount();
  for (;;) {
    updateLineSensors();
    lineDetectedStable = isLineDetectedStable();

    rawL = getRawL();  rawM = getRawM();  rawR = getRawR();
    avgL = getAvgL();  avgM = getAvgM();  avgR = getAvgR();

    vTaskDelayUntil(&lastWake, LINE_PERIOD_TICKS);
  }
}

 
// Ultrasonic Sensor Task
void vTaskUltrasonic(void *pv) {
  TickType_t lastWake = xTaskGetTickCount();
  for (;;) {
    updateUltrasonic();

    obstacleStable = isObstacleStable();
    d1 = getDist1();
    d2 = getDist2();

    vTaskDelayUntil(&lastWake, ULTRA_PERIOD_TICKS);
  }
}

 
// FSM Task : main robot logic
void vTaskFSM(void *pv) {
  TickType_t lastWake = xTaskGetTickCount();

  for (;;) {
    switch (state) {

      case FORWARD:
        moveForward(forwardSpeed);
        if (lineDetectedStable || obstacleStable) {
          stopCar();
          enterState(STOP1_AFTER_LINE);
        }
        break;

      case STOP1_AFTER_LINE:
        stopCar();
        if (millis() - stateStart >= settleStopMs) {
          enterState(REVERSE);
        }
        break;

      case REVERSE:
        moveBackward(reverseSpeed);
        if (millis() - stateStart >= reverseTimeMs) {
          stopCar();
          enterState(STOP2_BEFORE_TURN);
        }
        break;

      case STOP2_BEFORE_TURN:
        stopCar();
        if (millis() - stateStart >= settleStopMs) {

          resetTicks();

          // pick CW or CCW randomly
          if (random(0, 2) == 0)
            turnDirection = TURN_DIR_CW;
          else
            turnDirection = TURN_DIR_CCW;

          enterState(TURNING);
        }
        break;

      case TURNING:
        if (turnDirection == TURN_DIR_CW)
          turnCW(turnSpeed);
        else
          turnCCW(turnSpeed);
       
        // Use encoder ticks to decide when to stop
        long absR = labs(rTicks);
        long absL = labs(lTicks);
        long avgTicks = (absR + absL) / 2;
       
        if (maxTicks >= TURN_TICKS_TARGET) {
          stopCar();
          enterState(STOP3_AFTER_TURN);
        }
        break;

      case STOP3_AFTER_TURN:
        stopCar();
        if (millis() - stateStart >= settleStopMs) {
          lineDetectedStable = false;
          obstacleStable = false;
          enterState(FORWARD);
        }
        break;
    }

    vTaskDelayUntil(&lastWake, FSM_PERIOD_TICKS);
  }
}

 
// Watchdog Task 
void vTaskWatchdog(void *pv) {
  TickType_t lastWake = xTaskGetTickCount();
  for (;;) {
    resetWatchdog();
    vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(1000));
  }
}

 
// Debug Print Task 
void vTaskDebug(void *pv) {
  TickType_t lastWake = xTaskGetTickCount();
  for (;;) {
    Serial.println("====== DEBUG ======");
    
    Serial.print("STATE = ");
    Serial.println(state);

    Serial.print("Turn Direction = ");
    Serial.println(turnDirection == TURN_DIR_CW ? "CW" : "CCW");

    Serial.print("RAW LMR: ");
    Serial.print(rawL); Serial.print(" ");
    Serial.print(rawM); Serial.print(" ");
    Serial.println(rawR);

    Serial.print("AVG LMR: ");
    Serial.print(avgL); Serial.print(" ");
    Serial.print(avgM); Serial.print(" ");
    Serial.println(avgR);

    Serial.print("US cm: d1=");
    Serial.print(d1, 1);
    Serial.print("  d2=");
    Serial.println(d2, 1);

    Serial.print("lineDetectedStable = ");
    Serial.println(lineDetectedStable ? "YES" : "NO");

    Serial.print("obstacleStable     = ");
    Serial.println(obstacleStable ? "YES" : "NO");

    Serial.print("Encoder ticks R/L: ");
    Serial.print(rTicks); Serial.print(" / ");
    Serial.println(lTicks);

    Serial.println("====================\n");

    vTaskDelayUntil(&lastWake, DEBUG_PERIOD_TICKS);
  }
}

 
// Setup
void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  delay(300);

  initMotors();
  initLineSensors();
  initUltrasonic();
  initEncoders();
  initWatchdog(60000); // ~1 minute

  setLineThreshold(1000);
  randomSeed(analogRead(A0));

  xTaskCreate(vTaskLineSensors, "line",      256,  NULL, 3, NULL);
  xTaskCreate(vTaskUltrasonic,  "ultra",     512,  NULL, 2, NULL);
  xTaskCreate(vTaskFSM,         "fsm",       512,  NULL, 4, NULL);
  xTaskCreate(vTaskWatchdog,    "watchdog",  128,  NULL, 1, NULL);
  xTaskCreate(vTaskDebug,       "debug",    1024,  NULL, 1, NULL);

  vTaskStartScheduler();

  while (1) {
    Serial.println("ERROR: vTaskStartScheduler failed!");
    delay(1000);
  }
}

