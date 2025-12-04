#include <ArduinoFake.h>
#include <unity.h>

#include "LineSensors.h"
#include "MotorControl.h"

using namespace fakeit;

static bool lineStable = false;

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

// Script line sensor values: low → high (leading to line detection)
static int analogReadScript(uint8_t pin) {
    readCounter++;
    bool highPhase = (readCounter > 10);

    if (pin == A10) return highPhase ? 1200 : 95;
    if (pin == A9)  return highPhase ? 1300 : 105;
    if (pin == A8)  return highPhase ? 1100 : 88;
    return 90;
}

void test_integration_forward_line_stop() {
    ArduinoFakeReset();
    readCounter = 0;

    When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
    When(OverloadedMethod(ArduinoFake(), analogRead, int(uint8_t)))
        .AlwaysDo(analogReadScript);

    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), analogWrite)).AlwaysReturn();

    initLineSensors();
    setLineThreshold(1015);

    // Start with robot moving forward
    moveForward(200);

    // Simulate multiple cycles
    for (int i = 0; i < 25; ++i) {
        updateLineSensors();
        lineStable = isLineDetectedStable();
        if (lineStable) {
            stopCar();
            break;
        }
    }

    TEST_ASSERT_TRUE(lineStable);
}


