// CytronMotorDriver.h
#ifndef CYTRON_MOTOR_DRIVER_H
#define CYTRON_MOTOR_DRIVER_H

// Dummy mode value for constructor (not used in tests)
constexpr int PWM_DIR = 0;

// Minimal stub of CytronMD for unit testing
class CytronMD {
public:
    CytronMD(int, int, int) {}
    void setSpeed(int) {}
};

#endif
