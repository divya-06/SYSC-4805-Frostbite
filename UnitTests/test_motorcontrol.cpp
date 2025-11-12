#include <ArduinoFake.h>
#include <unity.h>

// We include MotorControl.h which itself includes CytronMotorDriver.h;
// our test adds -I test/fakes so the shim header is used.
#include "MotorControl.h"

using namespace fakeit;

void setUp() { ArduinoFakeReset(); }

void test_motorcontrol_api_invocations() {
  // Arrange base Arduino calls in case your implementation uses them
  When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
  When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
  When(Method(ArduinoFake(), analogWrite)).AlwaysReturn();

  // Act — smoke test public API
  initMotors();
  moveForward(120);
  moveBackward(100);
  turnCW(150);
  stopCar();

  // Assert — if we had DI into motor instances we could assert speeds.
  // Here we simply confirm no crashes and API linkage holds.
  TEST_ASSERT_TRUE(true);
}

int main(int, char**) {
  UNITY_BEGIN();
  RUN_TEST(test_motorcontrol_api_invocations);
  return UNITY_END();
}