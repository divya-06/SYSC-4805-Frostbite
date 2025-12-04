# Unit & Integration Test Branch README

This branch contains both the **unit testing** and **integration testing** environments for the Frostbite project. All test files are located in the `test/` directory, and the `platformio.ini` configuration file is located at the project root.

## Project Structure

```
Frostbite/
│
├── lib/
│   ├── Encoders/
│   │   ├── Encoders.cpp
│   │   └── Encoders.h
│   │
│   ├── LineSensors/
│   │   ├── LineSensors.cpp
│   │   └── LineSensors.h
│   │
│   ├── MotorControl/
│   │   ├── CytronMotorDriver.h
│   │   ├── MotorControl.cpp
│   │   └── MotorControl.h
│   │
│   ├── UltrasonicSensors/
│   │   ├── UltrasonicSensors.cpp
│   │   └── UltrasonicSensors.h
│   │
│   └── WatchdogTimer/
│       ├── WatchdogTimer.cpp
│       └── WatchdogTimer.h
│
├── src/
│   └── main.ino
│
├── test/
│   ├── README
│   ├── test_encoders.cpp
│   ├── test_linesensors.cpp
│   ├── test_motorcontrol.cpp
│   ├── test_ultrasonic.cpp
│   ├── test_watchdog.cpp
│   │
│   ├── test_full_integration.cpp          // FSM + sensors + encoders
│   ├── test_integration_forward_line_stop.cpp
│   ├── test_integration_obstacle_reverse.cpp
│   ├── test_integration_turn_until_ticks.cpp
│   │
│   └── test_runner.cpp
│
├── platformio.ini
└── README.md           // this file
```

## Environment Configuration

The `platformio.ini` defines two environments:

### 1. `env:due`

Used for compiling actual firmware for the Arduino Due. Uses the Arduino
framework and ArduinoFake library.

### 2. `env:native`

Used for running unit tests on your computer (no hardware needed). Uses
Unity test framework + ArduinoFake.

## How to Run Unit Tests

### 1. Install PlatformIO Core

    py -m pip install --user platformio

### 2. Run all tests using the native environment

Inside the project root:

    py -m platformio test -e native

This will compile and run all test files under the `test/` directory.

## Unit Test Overview

### test_encoders.cpp

Tests encoder tick reset and initialization behavior.

### test_linesensors.cpp

Tests line sensor thresholding, averaging, and stable detection logic.

### test_motorcontrol.cpp

Smoke‑tests the public motor control API to ensure no crashes.

### test_ultrasonic.cpp

Tests ultrasonic distance reading and stable obstacle detection.

### test_watchdog.cpp

Ensures watchdog initialization and kick functions are callable.

### test_runner.cpp

Registers and executes all unit test functions.

## Integration Test Overview

### test_full_integration.cpp
Contains higher-level tests that combine multiple modules and verify the FSM logic as a whole.

#### 1. test_full_integration_line_to_reverse() — Line → Stop → Reverse Sequence
Simulates rising line-sensor values and confirms that the FSM correctly transitions out of `FORWARD` into the `STOP1_AFTER_LINE` and `REVERSE` sequence.

#### 2. test_full_integration_turn_with_encoders() — Reverse → Turn → Forward Sequence
Simulates encoder ticks during turning and verifies full transitions through `STOP2_BEFORE_TURN`, `TURNING`, `STOP3_AFTER_TURN`, and back to `FORWARD`.

These tests ensure that the FSM, sensors, timing logic, and encoder feedback behave correctly as a combined system.

---

### test_integration_forward_line_stop.cpp

#### 3. test_integration_forward_line_stop() — Forward → Detect Line → Stop
Uses scripted analog readings that shift from low → high.
The robot updates line sensors repeatedly, detects a stable line, and calls `stopCar()`.
This verifies that line sensing and motor control interact correctly.

---

### test_integration_obstacle_reverse.cpp

#### 4. test_integration_obstacle_reverse() — Obstacle → Stable Detection → Reverse
Simulates ultrasonic readings using a fake `pulseIn()` and `millis()`.
Once the obstacle becomes stable, the robot responds by reversing.
This confirms that ultrasonic detection and motor reactions work together properly.

---

### test_integration_turn_until_ticks.cpp

#### 5. test_integration_turn_until_ticks() — Turn Until Tick Threshold
Resets encoder ticks, starts a clockwise turn, and simulates tick increments.
The goal is to verify that turning and encoder tracking operate correctly together in the native test environment.
