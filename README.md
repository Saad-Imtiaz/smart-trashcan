# SmartTrashCan 🗑️

## Overview 🌟
SmartTrashCan is an IoT-enabled project designed to make waste management smarter and more efficient. It utilizes an ESP8266 module along with ultrasonic sensors to monitor the fill level of trash cans, automatically reporting the status via ThingSpeak and Twitter.

## Hardware Requirements 🛠️
- ESP8266 WiFi Module
- Ultrasonic Sensor (HC-SR04)
- LEDs (Red and Green)
- Dip Switches (for bin selection)
- Basic electronic components (wires, breadboard, etc.)

## Software Dependencies 📦
- WiFiEsp Library
- ThingSpeak Library

## Setup and Configuration 📐
1. **Hardware Setup:**
   - Connect ultrasonic sensors and LEDs to the appropriate pins on the ESP8266.
   - Install dip switches for selecting different trash bins.

2. **Software Configuration:**
   - Rename `secrets_example.h` to `secrets.h`.
   - Update with your WiFi network details and ThingSpeak API keys.

3. **Code Deployment:**
   - Flash the provided code to your ESP8266 module.

## Features 🌈
- Monitors trash can fill levels in real-time.
- Utilizes LEDs for immediate visual status updates.
- Sends bin fullness alerts to Twitter and updates ThingSpeak channels.
- Supports multiple trash cans with configurable dip switches.

## How It Works 🔍
When a trash can is nearing full capacity:
- The red LED is activated.
- A notification is sent to Twitter.
- The ThingSpeak channel is updated with the latest status.
