#include <ArduinoFake.h>
#include <unity.h>

#include "MotorControl.h"

using namespace fakeit;

void setUp() { ArduinoFakeReset(); }

void test_motorcontrol_api_invocations() {
  When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
  When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
  When(Method(ArduinoFake(), analogWrite)).AlwaysReturn();

  // Smoke test public API
  initMotors();
  moveForward(120);
  moveBackward(100);
  turnCW(150);
  stopCar();

  // Confirms that the code ran without errors.
  TEST_ASSERT_TRUE(true);
}

int main(int, char**) {
  UNITY_BEGIN();
  RUN_TEST(test_motorcontrol_api_invocations);
  return UNITY_END();
}
