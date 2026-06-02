# IoT-based-Temperature-Control-System
This project is an ESP32-based smart LED control system that uses gesture input, temperature sensing, and Blynk IoT integration. The LED can be turned on or off using hand gestures detected by the APDS9960 gesture sensor or through a Blynk mobile/web dashboard.

When the LED is turned on, its brightness is automatically adjusted based on the ambient temperature measured by the BME280 sensor. Lower temperatures keep the LED dimmer, while higher temperatures increase the LED brightness. The system also sends live temperature readings to Blynk so users can monitor the environment remotely.

## Features

- Control LED using hand gestures
- UP gesture turns the LED on
- DOWN gesture turns the LED off
- Blynk virtual switch support for remote LED control
- Live temperature monitoring through Blynk
- Automatic LED brightness adjustment based on temperature
- PWM brightness control on ESP32
- Serial Monitor output for debugging sensor readings and gestures

## Hardware Used

- ESP32 board
- APDS9960 gesture sensor
- BME280 temperature sensor
- LED connected to PWM pin
- Blynk IoT platform
