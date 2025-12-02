#include <ArduinoFake.h>
#include <unity.h>
#include "LineSensors.h"

using namespace fakeit;

#ifndef A9
#define A9 9
#endif
#ifndef A8
#define A8 8
#endif
#ifndef A10
#define A10 10
#endif

static int readCounter = 0;

static int analogReadScript(uint8_t pin) {
    readCounter++;
    bool highPhase = (readCounter > 12);  // Switch to high values after warm-up

    if (pin == A10) return highPhase ? 1200 : 100;  // L
    if (pin == A9) return highPhase ? 1300 : 120;  // M
    if (pin == A8) return highPhase ? 1100 : 90;   // R
    return highPhase ? 1100 : 100;
}

void test_linesensors_threshold_and_stability() {
    ArduinoFakeReset();
    readCounter = 0;

    When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
    When(OverloadedMethod(ArduinoFake(), analogRead, int(uint8_t)))
        .AlwaysDo(analogReadScript);

    initLineSensors();
    setLineThreshold(1015);

    for (int i = 0; i < 30; ++i) {
        updateLineSensors();
    }

    TEST_ASSERT_TRUE(isLineDetectedStable());
    TEST_ASSERT_TRUE(getAvgL() >= 1015);
    TEST_ASSERT_TRUE(getAvgM() >= 1015);
    TEST_ASSERT_TRUE(getAvgR() >= 1015);
}
