#include <ArduinoFake.h>
#include <unity.h>
#include "Encoders.h"

using namespace fakeit;

void test_encoders_init_and_reset() {
  ArduinoFakeReset();
  When(Method(ArduinoFake(), pinMode)).AlwaysReturn();

  resetTicks();

  TEST_ASSERT_EQUAL(0, getRightTicks());
  TEST_ASSERT_EQUAL(0, getLeftTicks());
}
