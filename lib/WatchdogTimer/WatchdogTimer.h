// WatchdogTimer.h
#pragma once
#include <Arduino.h>

void initWatchdog(unsigned long timeoutMs);
void resetWatchdog();
