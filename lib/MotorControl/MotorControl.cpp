// MotorControl.cpp
#include "MotorControl.h"

// Motor instances
static CytronMD motor1(PWM_DIR, 34, 26);   // Right front
static CytronMD motor2(PWM_DIR, 36, 28);   // Right rear
static CytronMD motor3(PWM_DIR, 35, 30);   // Left rear
static CytronMD motor4(PWM_DIR, 37, 32);   // Left front

void initMotors() {
    // Reserved for future hardware setup
}

void moveForward(int v) {
    motor1.setSpeed(v);
    motor2.setSpeed(v);
    motor3.setSpeed(v);
    motor4.setSpeed(v);
}

void moveBackward(int v) {
    motor1.setSpeed(-v);
    motor2.setSpeed(-v);
    motor3.setSpeed(-v);
    motor4.setSpeed(-v);
}

void turnCW(int v) {
    // Right side backward, left side forward
    motor1.setSpeed(-v);
    motor2.setSpeed(-v);
    motor3.setSpeed(v);
    motor4.setSpeed(v);
}

void stopCar() {
    motor1.setSpeed(0);
    motor2.setSpeed(0);
    motor3.setSpeed(0);
    motor4.setSpeed(0);
}
