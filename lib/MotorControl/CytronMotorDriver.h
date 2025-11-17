// lib/MotorControl/CytronMotorDriver.h
#ifndef CYTRON_MOTOR_DRIVER_H
#define CYTRON_MOTOR_DRIVER_H

// Just something to pass as the first constructor argument.
// Its actual value doesn't matter for unit tests.
constexpr int PWM_DIR = 0;

// Minimal stub of the real CytronMD class
class CytronMD {
public:
    CytronMD(int /*mode*/, int /*pwmPin*/, int /*dirPin*/) {
        // Do nothing in the stub
    }

    void setSpeed(int /*speed*/) {
        // Do nothing in the stub; tests only care that this is called,
        // not that it actually drives real hardware.
    }
};

#endif // CYTRON_MOTOR_DRIVER_H
