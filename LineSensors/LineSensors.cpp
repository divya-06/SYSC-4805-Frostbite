// LineSensors.cpp
#include "LineSensors.h"

// Pins
#define LFS_L A7
#define LFS_M A5
#define LFS_R A6

// Parameters (can be overridden by setLineThreshold)
static int threshold = 1004;

// Moving average
#define WINDOW_SIZE 4
static uint16_t bufL[WINDOW_SIZE] = {0}, bufM[WINDOW_SIZE] = {0}, bufR[WINDOW_SIZE] = {0};
static uint32_t sumL = 0, sumM = 0, sumR = 0;
static uint8_t  wIdx = 0;
static bool filled = false;

// Debounce for line: require consecutive detections
const uint8_t DETECT_COUNT_REQ = 3;
static uint8_t detectCount = 0;

// Raw + avg storage
static uint16_t rawL = 0, rawM = 0, rawR = 0;
static uint16_t avgL = 0, avgM = 0, avgR = 0;

static inline uint16_t updateAvg(uint16_t *buf, uint32_t &sum, uint16_t newVal) {
  uint16_t old = buf[wIdx];
  sum -= old;
  buf[wIdx] = newVal;
  sum += newVal;
  uint8_t n = filled ? WINDOW_SIZE : (wIdx + 1);
  return (uint16_t)(sum / n);
}

void initLineSensors() {
  pinMode(LFS_L, INPUT);
  pinMode(LFS_M, INPUT);
  pinMode(LFS_R, INPUT);
}

void setLineThreshold(int t) {
  threshold = t;
}

void updateLineSensors() {
  // Read raw
  rawL = analogRead(LFS_L);
  rawM = analogRead(LFS_M);
  rawR = analogRead(LFS_R);

  // Moving average
  avgL = updateAvg(bufL, sumL, rawL);
  avgM = updateAvg(bufM, sumM, rawM);
  avgR = updateAvg(bufR, sumR, rawR);

  // Advance window index
  wIdx++;
  if (wIdx >= WINDOW_SIZE) {
    wIdx = 0;
    filled = true;
  }

  // Line detection with debounce
  bool lineDetectedNow = (avgL > threshold) || (avgM > threshold) || (avgR > threshold);

  if (lineDetectedNow) {
    if (detectCount < 255) detectCount++;
  } else {
    if (detectCount > 0) detectCount--;
  }
}

bool isLineDetectedStable() {
  return (detectCount >= DETECT_COUNT_REQ);
}

// Getters
uint16_t getRawL() { return rawL; }
uint16_t getRawM() { return rawM; }
uint16_t getRawR() { return rawR; }

uint16_t getAvgL() { return avgL; }
uint16_t getAvgM() { return avgM; }
uint16_t getAvgR() { return avgR; }
