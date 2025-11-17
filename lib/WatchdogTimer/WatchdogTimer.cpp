// WatchdogTimer.cpp
#include "WatchdogTimer.h"

#ifdef ARDUINO
#include <sam.h>

// Hardware watchdog (Arduino Due)
void initWatchdog(unsigned long timeoutMs) {
    uint32_t ticks = timeoutMs / 16;     // ~16 ms per tick
    if (ticks > 4095) ticks = 4095;      // 12-bit max

    WDT->WDT_MR =
        WDT_MR_WDRSTEN |                 // Reset on timeout
        WDT_MR_WDDBGHLT |                // Halt in debug mode
        WDT_MR_WDV(ticks);
}

void resetWatchdog() {
    WDT->WDT_CR = WDT_CR_KEY(0xA5) | WDT_CR_WDRSTT;
}

#else

// Native test stubs
void initWatchdog(unsigned long) {}
void resetWatchdog() {}

#endif
