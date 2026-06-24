# ESP32 Motion Tracking Telemetry System

## Project Description

An independent ESP32 based system that uses I2C communication protocols to gather accelerometer and gyroscope data from a MPU6500 6-axis motion tracking sensor and display that information in real-time on a SSD1315-driven OLED screen. Developed low-level drivers to process and map data vectors directly onto a portable display eliminating the need to stay near the user's console.

## Schematic

<img width="765" height="890" alt="Screenshot 2026-06-23 231418" src="https://github.com/user-attachments/assets/49789966-c35a-4aba-a7b9-6d06362bbc39" />

*Schematic is shown with an ESP32, MPU6050, and SSD1306-driven OLED screen. All hardware is compatible with original system and GPIO pins stay consistent.*

## Hardware and Software Requirements

### Hardware Requirements

* ESP32 Development Board
* MPU6500 6-Axis Motion Tracking Sensor
* SSD1315-Driven 128/64 Dot Matrix OLED Screen
* Breadboard, Jumper Wires, Micro-USB/USB-C

### Software Requirements

* Recommended: Visual Studio Code IDE with ESP-IDF Extension
* Espressif IoT Development Framework (ESP-IDF)
* CMake and Ninja Build Systems
* Windows 10/11, Linux, MacOS


### See [docs page](https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html#start-a-new-project) on how to build the environment for this system.
