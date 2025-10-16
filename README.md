# EAGLE-FIRMWARE

Dual setup repository for ESP32 firmware (ethical, educational):
- `platformio/` (Arduino framework via PlatformIO)
- `arduino-ide/` (Arduino IDE sketches)

Features (non-offensive):
- OLED UI (SSD1306 128x64 I2C), 4-button navigation
- Passive Wi‑Fi scan (SSIDs, RSSI, channel)
- Passive BLE scan (address, RSSI)
- Simple calculator app
- System screen (uptime, free heap, IP)

Hardware (defaults):
- ESP32 DevKit, OLED SSD1306 I2C at 0x3C
- Buttons: UP=GPIO32, DOWN=GPIO33, BACK=GPIO25, OK=GPIO26 (INPUT_PULLUP)

## PlatformIO
1. Open `platformio/` in VS Code with PlatformIO extension
2. Build and upload to the ESP32
3. Serial monitor: 115200 baud

Libraries (handled by PlatformIO): Adafruit GFX, Adafruit SSD1306, NimBLE-Arduino

## Arduino IDE
1. Install ESP32 board support (Boards Manager)
2. Install libraries: Adafruit GFX, Adafruit SSD1306, NimBLE-Arduino
3. Open `arduino-ide/EAGLE-FIRMWARE/EAGLE-FIRMWARE.ino`, select ESP32 Dev Module, upload

## Ethics and Scope
This repository is strictly for ethical and educational use. Offensive features (attacks, deauth, spam, BadUSB) are not implemented. Do not use the firmware for illegal activities.

## License
MIT License (see LICENSE)
