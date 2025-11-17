// WatchdogTimer.cpp
#include "WatchdogTimer.h"

#ifdef ARDUINO
#include <sam.h>   // Access to WDT registers on Arduino Due

// ======== REAL HARDWARE IMPLEMENTATION ========
void initWatchdog(unsigned long timeoutMs) {
    // Convert ms to watchdog ticks (~1 tick ≈ 16ms at 32kHz)
    uint32_t timeoutValue = (timeoutMs / 16);
    if (timeoutValue > 4095) timeoutValue = 4095;  // 12-bit max

    WDT->WDT_MR = 0x00000000      // Disable WDT in Wait Mode
                | WDT_MR_WDRSTEN  // Reset MCU on timeout
                | WDT_MR_WDDBGHLT // Pause in debug mode
                | WDT_MR_WDV(timeoutValue);
}

void resetWatchdog() {
    // Restart command
    WDT->WDT_CR = WDT_CR_KEY(0xA5) | WDT_CR_WDRSTT;
}

#else
// ======== NATIVE TEST BUILD (NO HARDWARE) ========

// Stub: do nothing, just satisfy linker
void initWatchdog(unsigned long timeoutMs) {
    (void)timeoutMs;
}

// Stub: do nothing
void resetWatchdog() {
}

#endif
