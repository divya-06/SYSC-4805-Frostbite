#include <ArduinoFake.h>
#include <unity.h>

using namespace fakeit;

// Forward declarations of test functions
void test_encoders_init_and_reset();
void test_linesensors_threshold_and_stability();
void test_motorcontrol_api_invocations();
void test_ultrasonic_stable_detection();
void test_watchdog_init_and_kick();

// Global Unity hooks (called before/after each test)
void setUp() {
  // If you want a global default, you can reset here,
  // but right now each test does its own local setup.
}

void tearDown() {
  // nothing for now
}

int main(int, char**) {
  UNITY_BEGIN();

  RUN_TEST(test_encoders_init_and_reset);
  RUN_TEST(test_linesensors_threshold_and_stability);
  RUN_TEST(test_motorcontrol_api_invocations);
  RUN_TEST(test_ultrasonic_stable_detection);
  RUN_TEST(test_watchdog_init_and_kick);

  return UNITY_END();
}
