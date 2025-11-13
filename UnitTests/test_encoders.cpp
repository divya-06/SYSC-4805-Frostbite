#include <ArduinoFake.h>
#include <unity.h>
#include "Encoders.h"

using namespace fakeit;

void setUp() { ArduinoFakeReset(); }

void test_encoders_init_and_reset() {
  When(Method(ArduinoFake(), pinMode)).AlwaysReturn();

  When(OverloadedMethod(ArduinoFake(), attachInterrupt, void(int, voidFuncPtr, int))).AlwaysReturn();

  initEncoders();
  resetTicks();
  TEST_ASSERT_EQUAL(0, getRightTicks());
  TEST_ASSERT_EQUAL(0, getLeftTicks());
}

int main(int, char**) {
  UNITY_BEGIN();
  RUN_TEST(test_encoders_init_and_reset);
  return UNITY_END();
}
