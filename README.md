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

1. Wire the SSD1306 OLED to the ESP32 over I2C: SDA to GPIO 21, SCL to GPIO 22 (set in `Wire.begin(21, 22)`). The sketch expects I2C address `0x3C`.
2. Install these libraries in the Arduino IDE or PlatformIO: Adafruit GFX, Adafruit SSD1306 and FluxGarage RoboEyes. NippleJS is loaded from a CDN by the web page, so the browser needs internet access for the joystick.
3. Open `perfect_dusty_eyes.ino` (the ESP32 sketch) and set your Wi-Fi name and password in the `ssid` and `password` constants near the top. The ESP32 joins that network as a client; it does not start its own access point.
4. Flash the sketch. The firmware does not print its IP address, so find it in your router's connected-devices list or your phone hotspot's client list. Adding `Serial.println(WiFi.localIP());` after the Wi-Fi connect loop prints it at 115200 baud.
5. Open that IP in a browser on the same network. The dashboard loads directly.

`esp8266_working_eyes.ino` is the ESP8266 variant of the same dashboard.

## What this demonstrates

Embedded C++ on ESP32, serving a real interactive web UI from a microcontroller, I2C display driving, and a REST-style control API on constrained hardware.

## Credits

Eye animation library: [FluxGarage RoboEyes](https://github.com/FluxGarage/RoboEyes). The web dashboard, endpoints and integration are my own.
