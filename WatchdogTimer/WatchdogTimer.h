// WatchdogTimer.h
#pragma once
#include <Arduino.h>

// Initialize the watchdog timer with a given timeout in milliseconds
void initWatchdog(unsigned long timeoutMs);

// Reset the watchdog to prevent system reset
void resetWatchdog();
