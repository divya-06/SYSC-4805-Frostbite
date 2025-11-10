#include <ArduinoFake.h>
#include <unity.h>

// Support either Encoders.h or Encoder.h (student repo variance)
#if __has_include("Encoders.h")
  #include "Encoders.h"
#elif __has_include("Encoder.h")
  #include "Encoder.h"
#else
  #error "Include Encoders.h or Encoder.h in include path"
#endif

using namespace fakeit;

void setUp() { ArduinoFakeReset(); }

void test_encoders_init_and_reset() {
  When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
  // Some repos use attachInterrupt(digitalPinToInterrupt(...), ...);
  When(OverloadedMethod(ArduinoFake(), attachInterrupt, void(int, voidFuncPtr, int))).AlwaysReturn();

  initEncoders();
  resetTicks();
  TEST_ASSERT_EQUAL(0, getRightTicks());
  TEST_ASSERT_EQUAL(0, getLeftTicks());

  // NOTE: For deeper testing, expose a test hook in your module to increment ticks
  // (e.g., a function that calls the internal ISR), then assert increments here.
}

int main(int, char**) {
  UNITY_BEGIN();
  RUN_TEST(test_encoders_init_and_reset);
  return UNITY_END();
}