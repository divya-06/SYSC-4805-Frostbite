#include <unity.h>
#include "WatchdogTimer.h"

void setUp(){}
void tearDown(){}

void test_watchdog_init_and_kick() {
  // This is a link/compile test under native.
  // In hardware, you'd verify WDT reset behavior; here we just ensure callable API.
  initWatchdog(1000);
  for (int i = 0; i < 5; ++i) {
    resetWatchdog();
  }
  TEST_ASSERT_TRUE(true);
}

int main(int, char**) {
  UNITY_BEGIN();
  RUN_TEST(test_watchdog_init_and_kick);
  return UNITY_END();
}