// MotorControl.cpp
#include "MotorControl.h"

// Your original motor mapping
static CytronMD motor1(PWM_DIR, 34, 26);   // Right front
static CytronMD motor2(PWM_DIR, 36, 28);   // Right rear  (encoder A2)
static CytronMD motor3(PWM_DIR, 35, 30);   // Left  rear  (encoder A3)
static CytronMD motor4(PWM_DIR, 37, 32);   // Left  front

void initMotors() {
  // Nothing special for Cytron, but keep for consistency/extension
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

void turnCW(int v) { // clockwise: right side backward, left side forward
  motor1.setSpeed(-v);  // Right front backward
  motor2.setSpeed(-v);  // Right rear  backward
  motor3.setSpeed( v);  // Left  rear  forward
  motor4.setSpeed( v);  // Left  front forward
}

void stopCar() {
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor3.setSpeed(0);
  motor4.setSpeed(0);
}
