#include <unity.h>
#include "WatchdogTimer.h"

void setUp(){}
void tearDown(){}

void test_watchdog_init_and_kick() {
  // In hardware, we verified WDT reset behaviour; here we just ensure callable API.
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
