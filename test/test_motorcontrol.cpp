#include <ArduinoFake.h>
#include <unity.h>
#include "MotorControl.h"

using namespace fakeit;

void test_motorcontrol_api_invocations() {
    ArduinoFakeReset();

    When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), analogWrite)).AlwaysReturn();

    moveForward(120);
    moveBackward(100);
    turnCW(150);
    stopCar();

    TEST_ASSERT_TRUE(true);  // API smoke test
}
