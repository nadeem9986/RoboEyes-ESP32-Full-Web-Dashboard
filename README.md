# RoboEyes ESP32 - Full Web Dashboard

A fully web-controlled OLED eye animation system for ESP32, built on the FluxGarage RoboEyes library. Every physical button is replaced by a browser-based control panel: moods, eye position, geometry, curiosity mode, flicker effects, cyclops mode, sweat/chill, and prebuilt animations, all updated in real time.

## Demo

_Add a short video or GIF here. A phone recording of the eyes reacting while you drag the joystick in the browser is enough - this is the section recruiters actually look at._

## Features

- ESP32-hosted web UI (HTML/CSS/JS), no app install needed
- Dark/light mode toggle
- Full mood control: Happy, Angry, Tired, Default
- 8-direction eye positioning with center reset
- NippleJS joystick for live eye movement
- Flicker effects (horizontal/vertical) with amplitude sliders
- Geometry shaping: width, height, border radius, spacing
- Curiosity, cyclops and sweat toggle modes
- Built-in animations: laugh, confused, blink
- REST-style endpoints for every action

## Hardware

- ESP32 dev board (any)
- 128x64 SSD1306 OLED display (I2C)
- Wi-Fi network the ESP32 and your browser can both reach

## Tech stack

- **Firmware:** C++, Arduino core for ESP32
- **Display:** Adafruit GFX + FluxGarage RoboEyes library
- **Frontend:** HTML, CSS, JavaScript, NippleJS (joystick)
- **Comms:** HTTP REST-style endpoints served by the ESP32

## Setup

1. Wire the SSD1306 OLED to the ESP32 over I2C (SDA/SCL).
2. Flash the firmware with the Arduino IDE or PlatformIO.
3. Connect the ESP32 to your Wi-Fi (or its own AP) and note the IP it prints.
4. Open that IP in any browser on the same network - the dashboard loads directly.

## What this demonstrates

Embedded C++ on ESP32, serving a real interactive web UI from a microcontroller, I2C display driving, and a REST-style control API on constrained hardware.

## Credits

Eye animation library: [FluxGarage RoboEyes](https://github.com/FluxGarage/RoboEyes). The web dashboard, endpoints and integration are my own.
