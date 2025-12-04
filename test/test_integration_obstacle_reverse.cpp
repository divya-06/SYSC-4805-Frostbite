#include <ArduinoFake.h>
#include <unity.h>

#include "UltrasonicSensors.h"
#include "MotorControl.h"

using namespace fakeit;

static unsigned long fakeMillisV = 0;

static unsigned long millisFake() {
    return fakeMillisV;
}

// Pulse returns a small distance (= obstacle close)
static unsigned long pulseInFake(uint8_t, uint8_t, unsigned long) {
    return 700; // ~12 cm
}

void test_integration_obstacle_reverse() {
    ArduinoFakeReset();
    fakeMillisV = 0;

    When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();

    When(OverloadedMethod(ArduinoFake(), millis, unsigned long()))
        .AlwaysDo(millisFake);

    When(OverloadedMethod(ArduinoFake(), delayMicroseconds, void(unsigned int)))
        .AlwaysReturn();

    When(OverloadedMethod(ArduinoFake(), pulseIn,
                          unsigned long(uint8_t, uint8_t, unsigned long)))
        .AlwaysDo(pulseInFake);

    initUltrasonic();

    // Run sensor updates
    for (int i = 0; i < 6; i++) {
        fakeMillisV += 60;
        updateUltrasonic();
    }

    TEST_ASSERT_TRUE(isObstacleStable());

    // Robot reacts by reversing
    moveBackward(180);

    TEST_ASSERT_TRUE(getDist1() > 0);
    TEST_ASSERT_TRUE(getDist2() > 0);
}


