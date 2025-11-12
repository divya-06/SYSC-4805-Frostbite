#include <ArduinoFake.h>
#include <unity.h>

// Some Arduino cores don't define Ax on native—ensure availability.
#ifndef A5
#define A5 5
#endif
#ifndef A6
#define A6 6
#endif
#ifndef A7
#define A7 7
#endif

#include "LineSensors.h"

using namespace fakeit;

static int readCounter = 0;

static int analogReadScript(uint8_t pin) {
  // First few cycles => below threshold, then above threshold to trigger stability.
  // Map pins to L/M/R as per your wiring (A7=L, A5=M, A6=R in your code).
  (void)pin;
  readCounter++;
  bool highPhase = (readCounter > 12); // after some cycles, switch to "line detected"
  if (pin == A7) return highPhase ? 1200 : 100; // L
  if (pin == A5) return highPhase ? 1300 : 120; // M
  if (pin == A6) return highPhase ? 1100 : 90;  // R
  return highPhase ? 1100 : 100;
}

void setUp() {
  ArduinoFakeReset();
  readCounter = 0;
}

void test_linesensors_threshold_and_stability() {
  // Arrange
  When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
  When(OverloadedMethod(ArduinoFake(), analogRead, int(uint8_t))).AlwaysDo(analogReadScript);

  initLineSensors();
  setLineThreshold(1004); // default threshold used in code

  // Act: run enough updates to fill moving average and debounce windows
  for (int i = 0; i < 30; ++i) {
    updateLineSensors();
  }

  // Assert
  TEST_ASSERT_TRUE(isLineDetectedStable());
  TEST_ASSERT_TRUE(getAvgL() >= 1004);
  TEST_ASSERT_TRUE(getAvgM() >= 1004);
  TEST_ASSERT_TRUE(getAvgR() >= 1004);
}

int main(int, char**) {
  UNITY_BEGIN();
  RUN_TEST(test_linesensors_threshold_and_stability);
  return UNITY_END();
}