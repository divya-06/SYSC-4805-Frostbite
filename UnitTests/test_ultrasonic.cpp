#include <ArduinoFake.h>
#include <unity.h>
#include "UltrasonicSensors.h"

using namespace fakeit;

static unsigned long fakeMillisVal = 0;
static int callIdx = 0;

static unsigned long millisFake() { return fakeMillisVal; }
static long pulseInFake(uint8_t, uint8_t, unsigned long) {
  // Far (~50cm => ~2915us) and near (~15cm => ~875us)
  callIdx++;
  return (callIdx % 2 == 0) ? 875 : 2915;
}
static void tick50Hz() { fakeMillisVal += 50; }

void setUp() { ArduinoFakeReset(); fakeMillisVal = 0; callIdx = 0; }

void test_ultrasonic_stable_detection() {
  When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
  When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
  When(OverloadedMethod(ArduinoFake(), millis, unsigned long())).AlwaysDo(millisFake);
  When(OverloadedMethod(ArduinoFake(), pulseIn, long(uint8_t, uint8_t, unsigned long))).AlwaysDo(pulseInFake);

  initUltrasonic();

  for (int i = 0; i < 10; ++i) { updateUltrasonic(); tick50Hz(); }
  
  callIdx = 0;
  for (int i = 0; i < 2; ++i) { updateUltrasonic(); tick50Hz(); }

  TEST_ASSERT_TRUE(isObstacleStable());
  TEST_ASSERT_TRUE(getDist1() > 0.0f);
  TEST_ASSERT_TRUE(getDist2() > 0.0f);
}

int main(int, char**) {
  UNITY_BEGIN();
  RUN_TEST(test_ultrasonic_stable_detection);
  return UNITY_END();
}
