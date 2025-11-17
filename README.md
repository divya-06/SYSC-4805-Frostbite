# SYSC-4805-Frostbite

## Overview
Frostbite is an autonomous snow-clearing robot designed for the SYSC 4805 Computer Systems Design Lab at Carleton University. 
The robot operates within a 6 m² arena bordered by black tape and autonomously detects and pushes simulated snow (wooden cubes) outside the boundary. 
It uses multiple sensors, motor control modules, and a finite state machine (FSM) to achieve fully autonomous operation.

## Features
- Fully autonomous motion using FSM control logic.
- Line detection for boundary avoidance.
- Ultrasonic obstacle detection for static and moving obstacles.
- Independent motor control with Cytron motor drivers.
- Encoder-based movement tracking.
- Modular software architecture for testing and integration.
- Watchdog timer for automatic recovery from software freezes.

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
- **Motor Driver:** Cytron MD10C x4  
- **Sensors:** 3x line sensors (analog), 2x ultrasonic sensors (HC-SR04)  
- **Encoders:** 2x wheel encoders  
- **Power Supply:** 12V Li-ion battery pack  

## How It Works
1. **FORWARD:** Robot drives forward until a boundary or obstacle is detected.  
2. **STOP:** Robot halts briefly to stabilize readings.  
3. **REVERSE:** Moves backward for a set time.  
4. **TURN CLOCKWISE:** Performs a 90° turn to find a new path.  
5. **REPEAT:** The cycle continues autonomously until the area is cleared.

## Setup Instructions
1. Clone this repository to your Arduino project folder.
2. Open `main.ino` in the Arduino IDE.
3. Connect your Arduino Due and select the correct COM port.
4. Upload the code to the board.
5. Power on the robot and place it in the test arena.

## Unit Testing
For all unit tests, switch to the unitTest branch and follow the instructions in its README.

## Authors
**Group 23 – Frostbite Team**  
- Divya Dushyanthan  
- Saja Fawagreh  
- Elizabeth Lorange  
- Vaanathy Thaneskumar  
