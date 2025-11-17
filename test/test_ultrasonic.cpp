#include <ArduinoFake.h>
#include <unity.h>
#include "UltrasonicSensors.h"

using namespace fakeit;

// Fake time + echo duration
static unsigned long fakeMillisVal = 0;

static unsigned long millisFake() {
    return fakeMillisVal;
}

// Always return "near" (~15 cm) so obstacle should be detected
static unsigned long pulseInFake(uint8_t, uint8_t, unsigned long) {
    // 15 cm ≈ 875 µs in your formula
    return 875u;
}

void test_ultrasonic_stable_detection() {
    // Fresh fake state
    ArduinoFakeReset();
    fakeMillisVal = 0;

    // Stub Arduino functions used in UltrasonicSensors.cpp
    When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();

    // delayMicroseconds(void(unsigned int)) – explicit overload
    When(OverloadedMethod(ArduinoFake(), delayMicroseconds,
                          void(unsigned int))).AlwaysReturn();

    When(OverloadedMethod(ArduinoFake(), millis,
                          unsigned long())).AlwaysDo(millisFake);

    When(OverloadedMethod(ArduinoFake(), pulseIn,
                          unsigned long(uint8_t, uint8_t, unsigned long)))
        .AlwaysDo(pulseInFake);

    // Init module
    initUltrasonic();

    // Simulate several update cycles; each cycle moves time forward
    for (int i = 0; i < 5; ++i) {
        fakeMillisVal += 60;   // > 50 ms so updateUltrasonic() runs each time
        updateUltrasonic();
    }

    // Now obstacle should be considered stable and distances non-zero
    TEST_ASSERT_TRUE(isObstacleStable());
    TEST_ASSERT_TRUE(getDist1() > 0.0f);
    TEST_ASSERT_TRUE(getDist2() > 0.0f);
}