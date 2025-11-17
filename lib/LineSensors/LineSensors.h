// LineSensors.h
#pragma once
#include <Arduino.h>

void initLineSensors();
void updateLineSensors();

uint16_t getRawL();
uint16_t getRawM();
uint16_t getRawR();

uint16_t getAvgL();
uint16_t getAvgM();
uint16_t getAvgR();

bool isLineDetectedStable();

void setLineThreshold(int t);
