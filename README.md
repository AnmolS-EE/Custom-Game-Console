# Custom ESP32 Arcade Console (V1.0)

An arcade-style game console built around the ESP32 microcontroller featuring a custom 2-layer PCB, OLED display, joystick input, tactile buttons, buzzer feedback, and multi-color LED's.

## Hardware Features
* **Microcontroller:** ESP32-WROOM-32
* **Display:** 0.96" I2C OLED Display
* **Controls:** 2-axis analog joystick + 4 tactile push buttons
* **Feedback:** Active buzzer for game audio + 4 status LEDs
* **Power:** Custom 2-layer PCB designed in KiCad

## Project Evolution: Breadboard to PCB
The project began as a fully functional breadboard prototype used to validate:

* GPIO pin mapping
* joystick input behavior
* display communication (I2C)
* game state transitions
* buzzer and LED feedback logic

After successful testing, the system was transferred to a custom PCB designed in KiCad, focusing on clean signal routing, reliable power delivery, and compact system integration. 

## Firmware & Logic
Firmware was developed using C++ (Arduino framework) on the ESP32 platform.
The software architecture supports multiple programmable games with modular input handling, display rendering, state management, and sound feedback. 

## Upcoming Improvements (V2)
* Improved PCB routing and component placement
* Better power trace optimization
* Cleaner schematic organization
* Enhanced manufacturability and debugging access
* More compact board layout
* Additional game support and firmware optimization
