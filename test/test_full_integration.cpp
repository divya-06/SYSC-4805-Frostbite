#include <ArduinoFake.h>
#include <unity.h>

#include "LineSensors.h"
#include "MotorControl.h"
// We won't call Ultrasonic/Encoders functions here to avoid native crashes

using namespace fakeit;

// -----------------------------------------------------------------------------
// Pin aliases (match LineSensors.cpp)
// -----------------------------------------------------------------------------
#ifndef A9
#define A9 9
#endif
#ifndef A8
#define A8 8
#endif
#ifndef A10
#define A10 10
#endif

#define LFS_L A9
#define LFS_M A8
#define LFS_R A10

// -----------------------------------------------------------------------------
// Local copy of enums & global state (same as main.ino, but defined HERE)
// -----------------------------------------------------------------------------
enum TurnDir {
  TURN_DIR_CW,
  TURN_DIR_CCW
};

enum State {
  FORWARD,
  STOP1_AFTER_LINE,
  REVERSE,
  STOP2_BEFORE_TURN,
  TURNING,
  STOP3_AFTER_TURN
};

// Shared “globals” for FSM + sensors (test-local)
static bool lineDetectedStable = false;
static bool obstacleStable     = false;

static uint16_t rawL = 0, rawM = 0, rawR = 0;
static uint16_t avgL = 0, avgM = 0, avgR = 0;
static long rTicks = 0, lTicks = 0;

static TurnDir        turnDirection = TURN_DIR_CW;
static State          state         = FORWARD;
static unsigned long  stateStart    = 0;

// Movement parameters (copy from main.ino)
static int forwardSpeed  = 255;
static int reverseSpeed  = 255;
static int turnSpeed     = 255;

static unsigned long reverseTimeMs = 500;
static unsigned long settleStopMs  = 150;

static const long TURN_TICKS_TARGET = 11; // ~135°

// -----------------------------------------------------------------------------
// Fake time base
// -----------------------------------------------------------------------------
static unsigned long fakeNow = 0;  // we control "time" manually in tests

static void enterState(State s) {
  state = s;
  stateStart = fakeNow;
}

// -----------------------------------------------------------------------------
// Local copy of FSM logic (single step, no FreeRTOS, NO ultrasonic/encoders calls)
// -----------------------------------------------------------------------------
static void fsmStepOnce() {
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
      if (fakeNow - stateStart >= settleStopMs) {
        enterState(REVERSE);
      }
      break;

    case REVERSE:
      moveBackward(reverseSpeed);
      if (fakeNow - stateStart >= reverseTimeMs) {
        stopCar();
        enterState(STOP2_BEFORE_TURN);
      }
      break;

    case STOP2_BEFORE_TURN:
      stopCar();
      if (fakeNow - stateStart >= settleStopMs) {
        // For tests: no real encoder reset, just zero our counters
        rTicks = 0;
        lTicks = 0;
        turnDirection = TURN_DIR_CW; // deterministic
        enterState(TURNING);
      }
      break;

    case TURNING:
    {
      if (turnDirection == TURN_DIR_CW)
        turnCW(turnSpeed);
      else
        turnCCW(turnSpeed);

      long absR = labs(rTicks);
      long absL = labs(lTicks);
      long avgTicks = (absR + absL) / 2;

      if (avgTicks >= TURN_TICKS_TARGET) {
        stopCar();
        enterState(STOP3_AFTER_TURN);
      }
      break;
    }

    case STOP3_AFTER_TURN:
      stopCar();
      if (fakeNow - stateStart >= settleStopMs) {
        lineDetectedStable = false;
        obstacleStable     = false;
        enterState(FORWARD);
      }
      break;
  }
}

// -----------------------------------------------------------------------------
// Faked sensor readings (line appears after some samples)
// -----------------------------------------------------------------------------
static int readCounter = 0;
static int analogReadScript(uint8_t pin) {
  readCounter++;
  bool highPhase = (readCounter > 15);  // after 15 reads, "line" appears

  if (pin == LFS_L) return highPhase ? 1200 : 100;
  if (pin == LFS_M) return highPhase ? 1300 : 120;
  if (pin == LFS_R) return highPhase ? 1100 : 90;
  return 100;
}

// -----------------------------------------------------------------------------
// TEST 1: Forward → detects line → STOP1 → REVERSE/STOP2
// -----------------------------------------------------------------------------
void test_full_integration_line_to_reverse() {
  ArduinoFakeReset();
  fakeNow     = 0;
  readCounter = 0;

  // Fake Arduino low-level functions
  When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
  When(OverloadedMethod(ArduinoFake(), analogRead, int(uint8_t)))
      .AlwaysDo(analogReadScript);
  When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
  When(Method(ArduinoFake(), analogWrite)).AlwaysReturn();

  // Init ONLY line sensors (they use analogRead + pinMode)
  initLineSensors();

  // Initial FSM state
  state              = FORWARD;
  lineDetectedStable = false;
  obstacleStable     = false;
  rTicks = lTicks    = 0;
  stateStart         = 0;

  setLineThreshold(1015);

  // Simulate ~30 control cycles
  for (int i = 0; i < 30; ++i) {
    // 1) update line sensors similar to the task
    updateLineSensors();
    lineDetectedStable = isLineDetectedStable();

    // 2) obstacle stays false in this test
    obstacleStable = false;

    // 3) advance fake time (5 ms per cycle)
    fakeNow += 5;

    // 4) run one FSM step
    fsmStepOnce();
  }

  // We should no longer be in FORWARD once line is detected + debounced
  TEST_ASSERT_NOT_EQUAL(FORWARD, state);

  // Likely in STOP1_AFTER_LINE, REVERSE, or STOP2_BEFORE_TURN
  TEST_ASSERT_TRUE(
    state == STOP1_AFTER_LINE ||
    state == REVERSE ||
    state == STOP2_BEFORE_TURN
  );
}

// -----------------------------------------------------------------------------
// TEST 2: From STOP2_BEFORE_TURN → TURNING → STOP3 → FORWARD
// -----------------------------------------------------------------------------
void test_full_integration_turn_with_encoders() {
  ArduinoFakeReset();
  fakeNow     = 0;
  readCounter = 0;

  When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
  When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
  When(Method(ArduinoFake(), analogWrite)).AlwaysReturn();

  // Pretend we just finished reversing and are ready to turn
  state              = STOP2_BEFORE_TURN;
  stateStart         = fakeNow;
  lineDetectedStable = true;   // still on the line
  obstacleStable     = false;
  rTicks             = 0;
  lTicks             = 0;

  // After settleStopMs, FSM should enter TURNING
  fakeNow += 200;  // > 150 ms
  fsmStepOnce();
  TEST_ASSERT_EQUAL(TURNING, state);

  // Simulate ticks increasing while turning
  for (int i = 0; i < 20; ++i) {
    rTicks += 1;
    lTicks += 1;
    fsmStepOnce();
    if (state == STOP3_AFTER_TURN) break;
  }

  TEST_ASSERT_EQUAL(STOP3_AFTER_TURN, state);

  // After STOP3 settle, we should go back to FORWARD and clear flags
  fakeNow += 200;  // > 150 ms
  fsmStepOnce();

  TEST_ASSERT_EQUAL(FORWARD, state);
  TEST_ASSERT_FALSE(lineDetectedStable);
  TEST_ASSERT_FALSE(obstacleStable);
}
