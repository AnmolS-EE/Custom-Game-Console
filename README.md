# Custom ESP32 Arcade Console (V1.0)

An arcade-style game console built around the ESP32 microcontroller featuring a custom 2-layer PCB, OLED display, joystick input, tactile buttons, buzzer feedback, and multi-color LED's.

## Hardware Features
* **Microcontroller:** ESP32-WROOM-32
* **Display:** 0.96" I2C OLED Display
* **Controls:** 2-axis analog joystick + 4 tactile push buttons
* **Feedback:** Active buzzer for game audio + 4 status LEDs
* **Power:** Custom 2-layer PCB designed in KiCad

### Schematic
![Circuit Schematic](https://github.com/AnmolS-EE/Custom-Game-Console/blob/main/media/Schematic.png)

### 3D PCB View
![3D PCB View](https://github.com/AnmolS-EE/Custom-Game-Console/blob/main/media/GameConsole_ESP32.png)

## Project Evolution: Breadboard to PCB
The project began as a fully functional breadboard prototype used to validate:

* GPIO pin mapping
* joystick input behavior
* display communication (I2C)
* game state transitions
* buzzer and LED feedback logic

### Breadboard Prototype
![Breadboard Prototype](https://github.com/AnmolS-EE/Custom-Game-Console/blob/main/media/breadboard.jpg)](https://github.com/AnmolS-EE/Custom-Game-Console/blob/main/media/PCB_Front.jpeg)

After successful testing, the system was transferred to a custom PCB designed in KiCad, focusing on clean signal routing, reliable power delivery, and compact system integration. 

### Assembled Hardware
![Fully Assembled PCB](https://github.com/AnmolS-EE/Custom-Game-Console/blob/main/media/PCB_Front.jpeg)

The final manufactured board has been created and the components have been soldered on. This transformed the initial breadboard concept into a robust, standalone device. You can watch a small edit of the soldering and final assembly process in my [TikTok build video](https://www.tiktok.com/@anmol.ee/video/7637941595454639381?is_from_webapp=1&sender_device=pc&web_id=7633530922164045320).

## Firmware & Logic
Firmware was developed using C++ (Arduino framework) on the ESP32 platform.
The software architecture supports multiple programmable games with modular input handling, display rendering, state management, and sound feedback.

## Future Improvements (V2.0)
While version 1.0 successfully validates the core hardware and software architecture, future versions will focus on a more clean and consumer-ready design. Planned improvements include:

* **Component Integration:** Transitioning away from bulky pre-packaged modules by designing custom footprints to mount a bare joystick and buzzer directly onto the PCB.
* **Enhanced Display:** Upgrading to a larger OLED screen for improved visibility and addition of color.
* **Layout Optimization:** Refining the PCB trace routing and component placement for an even cleaner and more efficient board design.
* **Custom Enclosure:** Designing and 3D printing a custom case to house the console, improve handling, and protect the exposed circuitry.
