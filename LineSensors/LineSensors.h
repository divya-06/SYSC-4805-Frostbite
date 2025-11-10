// LineSensors.h
#pragma once
#include <Arduino.h>

// Call once in setup()
void initLineSensors();

// Call every loop to update readings
void updateLineSensors();

// Getters for debug / logic
uint16_t getRawL();
uint16_t getRawM();
uint16_t getRawR();

uint16_t getAvgL();
uint16_t getAvgM();
uint16_t getAvgR();

bool isLineDetectedStable();

// Adjust threshold for line detection
void setLineThreshold(int t);
