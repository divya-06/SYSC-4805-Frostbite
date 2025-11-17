# Unit Test Branch README

This branch contains the unit testing environment for the Frostbite
project. All test files are located in the `test/` directory, and the
`platformio.ini` configuration file is located at the project root.

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
│   └── main.cpp      // not used in native tests
│
├── test/
│   ├── README
│   ├── test_encoders.cpp
│   ├── test_linesensors.cpp
│   ├── test_motorcontrol.cpp
│   ├── test_runner.cpp
│   ├── test_ultrasonic.cpp
│   └── test_watchdog.cpp
│
├── platformio.ini
└── README.md         // this file

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

## Purpose of Test Files

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
