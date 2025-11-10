// WatchdogTimer.cpp
#include "WatchdogTimer.h"
#include <sam.h>

void initWatchdog(unsigned long timeoutMs) {
  // Convert ms to watchdog ticks (~1 tick = 16ms at 32kHz clock)
  uint32_t timeoutValue = (timeoutMs / 16);
  if (timeoutValue > 4095) timeoutValue = 4095;  // max limit for 12-bit counter

  WDT->WDT_MR = 0x00008000    // Disable Watchdog in Wait Mode
              | WDT_MR_WDRSTEN // Enable reset on timeout
              | WDT_MR_WDDBGHLT // Halt in debug
              | WDT_MR_WDV(timeoutValue); // Set counter value
}

void resetWatchdog() {
  // Write the restart command key
  WDT->WDT_CR = WDT_CR_KEY(0xA5) | WDT_CR_WDRSTT;
}
