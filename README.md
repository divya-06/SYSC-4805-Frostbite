# SYSC-4805-Frostbite

## Overview
Frostbite is an autonomous snow-clearing robot designed for the SYSC 4805 Computer Systems Design Lab at Carleton University.
The robot operates within a 6 m² arena bordered by black tape and autonomously detects and pushes simulated snow (wooden cubes) outside the boundary.
It uses a combination of line sensors, ultrasonic sensors, motor drivers, encoders, FreeRTOS tasks, and a finite state machine (FSM) to achieve fully autonomous operation.

## Features
- Fully autonomous motion using an FSM-based control architecture.
- Boundary detection using three analog line sensors.
- Obstacle detection using ultrasonic sensing with stability filtering.
- Independent four‑motor control using Cytron MD10C drivers.
- Encoder feedback for accurate motion tracking.
- Modular and test‑friendly codebase.
- Hardware watchdog timer for automatic recovery from software hangs.

## Project Structure
```
/SYSC-4805-Frostbite
│
├── MotorControl/
│   ├── MotorControl.h
│   └── MotorControl.cpp
│
├── LineSensors/
│   ├── LineSensors.h
│   └── LineSensors.cpp
│
├── UltrasonicSensors/
│   ├── UltrasonicSensors.h
│   └── UltrasonicSensors.cpp
│
├── Encoders/
│   ├── Encoders.h
│   └── Encoders.cpp
│
├── WatchdogTimer/
│   ├── WatchdogTimer.h
│   └── WatchdogTimer.cpp
│
└── main.ino
```

## Hardware Components
- **Microcontroller:** Arduino Due  
- **Motor Drivers:** Cytron MD10C × 4  
- **Sensors:**  
  - 3× analog line sensors  
  - 3× ultrasonic sensors (HC‑SR04)  
- **Encoders:** 2× rear wheel encoders  
- **Power Supply:** 5-AA battery pack  

## How It Works
The robot continuously runs several FreeRTOS tasks:
1. **Line Sensor Task:** Reads three line sensors and determines if a boundary is detected.  
2. **Ultrasonic Task:** Measures distance from obstacles and stabilizes noisy readings.  
3. **FSM Task:** Controls the robot’s movement based on sensor inputs.  
4. **Watchdog Task:** Resets the watchdog every second to ensure system health.  
5. **Debug Task:** Prints system diagnostics periodically.

### FSM Motion Cycle
1. **FORWARD:** Move forward until a boundary or obstacle is detected.  
2. **STOP:** Pause briefly for sensor stabilization.  
3. **REVERSE:** Move backward for a fixed interval.  
4. **TURN (CW or CCW):** Randomly select a direction to avoid predictable behavior.  
5. **STOP:** Stabilize.  
6. **LOOP BACK:** Resume forward motion.

## Setup Instructions
1. Clone this repository.  
2. Open `main.ino` in the Arduino IDE.  
3. Select **Arduino Due (Programming Port)** and correct COM port.  
4. Upload the project.  
5. Place the robot in the test arena and power it on.

## Unit Testing
Unit tests are located in the `tests` branch.  
Switch to that branch and follow the README instructions to test individual modules.

## Authors
**Group 23 – Frostbite Team**  
- Divya Dushyanthan  
- Saja Fawagreh  
- Elizabeth Lorange  
- Vaanathy Thaneskumar  
