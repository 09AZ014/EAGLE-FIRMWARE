# EAGLE-FIRMWARE

**Advanced ESP32 Pentesting Firmware** - Inspired by Bruce with enhanced features

Dual setup repository for ESP32 firmware:
- `platformio/` (Arduino framework via PlatformIO - **RECOMMENDED**)
- `arduino-ide/` (Arduino IDE sketches)

## 🎯 Features (Offensive Security Tools)

### 📡 WiFi Attacks
- ✅ **Beacon Spam**: Flood area with fake WiFi access points (IMPLEMENTED)
- ✅ **Deauth Attack**: Disconnect specific clients from WiFi networks (IMPLEMENTED)
- ✅ **Deauth Flood**: Mass deauthentication attack on all nearby networks (IMPLEMENTED)
- ✅ **Evil Portal**: Captive portal for credential harvesting (IMPLEMENTED)
- ✅ **WiFi Scan**: Advanced network reconnaissance (IMPLEMENTED)
- ⏸️ **Raw Sniffer**: Capture WiFi packets (Coming soon)
- ⏸️ **Wardriving**: GPS-enabled WiFi mapping (Requires GPS module - Coming soon)

### 🔵 BLE Attacks
- ✅ **BLE Spam**: Flood nearby devices with fake BLE advertisements (IMPLEMENTED)
  - ✅ iOS Spam (Apple Juice): AirPods, Beats, and other Apple devices
  - ✅ Android Spam (Fast Pair): Google Pixel Buds, JBL, Sony, Bose devices
  - ✅ Samsung Spam: Galaxy Buds series
  - ✅ Windows Spam (Swift Pair): Surface headphones and accessories
- ✅ **BLE Scan**: Advanced Bluetooth Low Energy reconnaissance (IMPLEMENTED)
- ⏸️ **Bad BLE**: Bluetooth-based BadUSB attacks (Coming soon)
- ⏸️ **BLE Keyboard**: Use ESP32 as wireless HID keyboard (Coming soon)

### 📻 RF Attacks (with CC1101 module)
- ⏸️ **RF Scan/Copy/Replay**: Capture and replay Sub-GHz signals (Requires CC1101 - Coming soon)
- ⏸️ **RF Jammer**: Jam RF frequencies (Requires CC1101 - Coming soon)
- ⏸️ **RF Spectrum Analyzer**: Visualize RF spectrum (Requires CC1101 - Coming soon)
- ⏸️ **Custom SubGhz**: Load and transmit custom RF payloads (Requires CC1101 - Coming soon)

### 💳 RFID/NFC Attacks (with PN532 module)
- ⏸️ **Read/Clone**: Read and clone RFID/NFC tags (Requires PN532 - Coming soon)
- ⏸️ **Write**: Write data to RFID/NFC tags (Requires PN532 - Coming soon)
- ⏸️ **Emulate**: Emulate RFID/NFC tags (Requires PN532 - Coming soon)
- ⏸️ **NDEF**: Write NDEF records (Requires PN532 - Coming soon)
- ⏸️ **Amiibolink**: Nintendo Amiibo emulation (Requires PN532 - Coming soon)

### 📺 IR Attacks
- ✅ **TV-B-Gone**: Universal TV remote control jammer (IMPLEMENTED - NEC protocol)
- ⏸️ **IR Receiver/Transmitter**: Capture and replay IR signals (Coming soon)
- ⏸️ **Protocols**: NEC (✅), SIRC, Samsung32, RC5, RC6 (Coming soon)

### 📶 Other Features
- ⏸️ **BadUSB**: Run Ducky scripts via USB (Coming soon - Requires USB capable board)
- ⏸️ **JavaScript Interpreter**: Run custom scripts on-device (Coming soon)
- ⏸️ **ESP-NOW**: Device-to-device communication (Coming soon)
- ✅ **WebUI**: REST API interface for remote control (IMPLEMENTED)
- ✅ **File Manager**: LittleFS management (IMPLEMENTED)
- ⏸️ **QR Codes**: Generate and display QR codes (Coming soon)

## 🔧 Hardware (defaults)
- ESP32 DevKit (standard), M5StickC Plus 2, or compatible boards
- Optional modules:
  - CC1101 (Sub-GHz RF)
  - PN532 (RFID/NFC)
  - GPS module (for wardriving)
  - SD Card reader

## 🚀 Installation

### Option 1: PlatformIO (Recommended)
1. Install [VS Code](https://code.visualstudio.com/) + [PlatformIO extension](https://platformio.org/install/ide?install=vscode)
2. Open `platformio/` folder in VS Code
3. Connect your ESP32 via USB
4. Click PlatformIO: Upload (or use `pio run -t upload`)
5. Open Serial Monitor at 115200 baud to view output

All libraries are automatically managed by PlatformIO (NimBLE-Arduino, ESP32 WiFi/BLE, etc.)

### Option 2: Arduino IDE
1. Install [Arduino IDE 2.x](https://www.arduino.cc/en/software)
2. Add ESP32 board support:
   - Go to `File > Preferences`
   - Add to "Additional Board Manager URLs": `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Install "esp32" by Espressif in Boards Manager
3. Install required libraries via Library Manager:
   - NimBLE-Arduino (h2zero)
   - ArduinoJson (Benoit Blanchon)
   - IRremoteESP8266 (David Conran)
4. Open `arduino-ide/EAGLE-FIRMWARE/EagleFirmware.ino`
5. Select board: `ESP32 Dev Module` (or M5StickC Plus 2)
6. Upload to ESP32

For detailed Arduino IDE instructions, see [arduino/README_ARDUINO.md](EAGLE-FIRMWARE/arduino/README_ARDUINO.md)

## 🔍 Usage

### Serial Commands
Connect via Serial Monitor (115200 baud) and use commands to trigger attacks:
```
# WiFi Attacks
wifi_scan              - Scan WiFi networks
wifi_deauth_flood      - Start deauth flood attack
wifi_beacon_spam       - Start beacon spam attack

# BLE Attacks
ble_spam_all           - Spam all BLE device types
ble_spam_ios           - Spam iOS devices (Apple Juice)
ble_spam_android       - Spam Android devices (Fast Pair)
ble_spam_samsung       - Spam Samsung devices
ble_spam_windows       - Spam Windows devices (Swift Pair)
ble_scan               - Scan BLE devices

# IR Attacks
ir_tvbgone             - TV-B-Gone (power off TVs)

# Evil Portal
evil_portal            - Start captive portal
portal_status          - Portal status
portal_dump            - Dump captured credentials

# Control
stop                   - Stop all attacks
status                 - System status
help                   - Show all commands
```

### Web Interface
When connected to WiFi, access the web interface:
1. Connect ESP32 to your WiFi (configure in code or via serial)
2. Note the IP address shown in Serial Monitor
3. Open browser and navigate to the IP address
4. Control all attacks via web interface

## ⚠️ Disclaimer & Legal Notice

**IMPORTANT**: This firmware is designed for **authorized penetration testing, security research, and educational purposes ONLY**.

### Legal Obligations:
- ❌ **NEVER** use this tool on networks, devices, or systems you don't own or have explicit written permission to test
- ❌ Unauthorized WiFi deauthentication attacks are **ILLEGAL** in most countries (FCC Part 15, GDPR, Computer Misuse Act, etc.)
- ❌ Unauthorized access to computer systems and networks is a **CRIMINAL OFFENSE**
- ✅ Only use on your own equipment or with explicit authorization from the owner
- ✅ Ensure compliance with local laws and regulations

### Usage Guidelines:
- Use in isolated, controlled environments (e.g., your own lab, Faraday cage)
- Obtain proper authorization before any security testing
- Follow responsible disclosure practices
- Respect privacy and do not intercept communications without consent

**By using this firmware, you agree to:**
1. Use it only for legal, authorized purposes
2. Take full responsibility for your actions
3. Hold the developers harmless from any misuse

The developers of EAGLE-FIRMWARE assume **NO LIABILITY** for misuse. Use at your own risk.

## 👤 Credits

EAGLE-FIRMWARE is inspired by and incorporates techniques from:
- **[Bruce](https://github.com/pr3y/Bruce)** by pr3y - Extensive ESP32 offensive framework
- **[ESP32Marauder](https://github.com/justcallmekoko/ESP32Marauder)** by justcallmekoko - WiFi/BLE attack toolkit
- **[esp32-wifi-penetration-tool](https://github.com/risinek/esp32-wifi-penetration-tool)** by risinek

Massive thanks to these projects and their contributors for pioneering ESP32-based security tools.

## 📝 License

MIT License (see LICENSE)

**Note**: While the code is MIT licensed, you are legally and ethically bound to use it responsibly.
