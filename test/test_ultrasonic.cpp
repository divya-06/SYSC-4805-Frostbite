#include <ArduinoFake.h>
#include <unity.h>
#include "UltrasonicSensors.h"

using namespace fakeit;

static unsigned long fakeMillisVal = 0;

static unsigned long millisFake() {
    return fakeMillisVal;
}

static unsigned long pulseInFake(uint8_t, uint8_t, unsigned long) {
    return 875u;   // ~15 cm
}

void test_ultrasonic_stable_detection() {
    ArduinoFakeReset();
    fakeMillisVal = 0;

    When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();

    When(OverloadedMethod(ArduinoFake(), delayMicroseconds,
                          void(unsigned int))).AlwaysReturn();

    When(OverloadedMethod(ArduinoFake(), millis,
                          unsigned long())).AlwaysDo(millisFake);

    When(OverloadedMethod(ArduinoFake(), pulseIn,
                          unsigned long(uint8_t, uint8_t, unsigned long)))
        .AlwaysDo(pulseInFake);

    initUltrasonic();

    for (int i = 0; i < 5; ++i) {
        fakeMillisVal += 60;
        updateUltrasonic();
    }

    TEST_ASSERT_TRUE(isObstacleStable());
    TEST_ASSERT_TRUE(getDist1() > 0.0f);
    TEST_ASSERT_TRUE(getDist2() > 0.0f);
}
