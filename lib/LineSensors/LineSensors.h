#pragma once
#include <Arduino.h>

 
// Setup
void initLineSensors();

 
// Update
void updateLineSensors();

 
// Raw Values
uint16_t getRawL();
uint16_t getRawM();
uint16_t getRawR();

 
// Averaged Values
uint16_t getAvgL();
uint16_t getAvgM();
uint16_t getAvgR();

 
// Stable Detection
bool isLineDetectedStable();

 
// Threshold
void setLineThreshold(int t);
