#include <ArduinoFake.h>
#include <unity.h>

using namespace fakeit;

void test_encoders_init_and_reset();
void test_linesensors_threshold_and_stability();
void test_motorcontrol_api_invocations();
void test_ultrasonic_stable_detection();
void test_watchdog_init_and_kick();

void test_full_integration_line_to_reverse();
void test_full_integration_turn_with_encoders();

void setUp() {}
void tearDown() {}

int main(int, char**) {
    UNITY_BEGIN();

    RUN_TEST(test_encoders_init_and_reset);
    RUN_TEST(test_linesensors_threshold_and_stability);
    RUN_TEST(test_motorcontrol_api_invocations);
    RUN_TEST(test_ultrasonic_stable_detection);
    RUN_TEST(test_watchdog_init_and_kick);

    RUN_TEST(test_full_integration_line_to_reverse);
    RUN_TEST(test_full_integration_turn_with_encoders);

    return UNITY_END();
}
