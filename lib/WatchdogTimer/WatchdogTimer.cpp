#include "WatchdogTimer.h"

#ifdef ARDUINO
#include <sam.h>

 
// Setup Watchdog
void initWatchdog(unsigned long timeoutMs) {
  uint32_t timeoutValue = (timeoutMs / 16);   // ~16ms per tick
  if (timeoutValue > 4095) timeoutValue = 4095; // ~65 seconds

  WDT->WDT_MR =
      0x00008000        |   // Disable in wait mode
      WDT_MR_WDRSTEN    |   // Reset on timeout
      WDT_MR_WDDBGHLT   |   // Halt in debug
      WDT_MR_WDV(timeoutValue);
}

 
// Reset Watchdog
void resetWatchdog() {
  WDT->WDT_CR = WDT_CR_KEY(0xA5) | WDT_CR_WDRSTT;
}

#else

// Native test stubs
void initWatchdog(unsigned long) {}

void resetWatchdog() {}

#endif