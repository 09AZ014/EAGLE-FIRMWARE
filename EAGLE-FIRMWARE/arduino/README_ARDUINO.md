# EAGLE-FIRMWARE - Arduino IDE Version

## 📋 Prerequisites

### Arduino IDE
- Arduino IDE 2.x or higher
- ESP32 Core 3.x installed

### ESP32 Core Installation
1. Open the Arduino IDE
2. Go to `File > Preferences`
3. In "Additional Boards Manager URLs" add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to `Tools > Board > Boards Manager`
5. Search for "ESP32" and install "esp32" by Espressif Systems

### Required Libraries
Install the following libraries via Library Manager:

#### Mandatory Libraries
- **ArduinoJson** by Benoit Blanchon
- **NimBLE-Arduino** by h2zero
- **IRremoteESP8266** by David Conran
- **WebServer** (already included with ESP32 Core)

#### Libraries for M5StickC Plus 2
- **M5StickCPlus2** by M5Stack
- **LovyanGFX** by lovyan03

#### Optional Libraries
- **LittleFS_esp32** by lorol
- **SD** (already included with ESP32 Core)
- **Wire** (already included with ESP32 Core)
- **SPI** (already included with ESP32 Core)

## 🔧 Board Setup

### 1. Board Configuration

#### For ESP32 DevKit:
1. `Tools > Board > ESP32 Arduino > ESP32 Dev Module`
2. `Tools > Upload Speed > 921600`
3. `Tools > CPU Frequency > 240MHz (WiFi/BT)`
4. `Tools > Flash Frequency > 80MHz`
5. `Tools > Flash Mode > QIO`
6. `Tools > Flash Size > 4MB (32Mb)`
7. `Tools > Partition Scheme > Default 4MB with spiffs`
8. `Tools > Core Debug Level > None`
9. `Tools > PSRAM > Enabled`

#### For M5StickC Plus 2:
1. `Tools > Board > ESP32 Arduino > M5Stick-C-Plus2`
2. `Tools > Upload Speed > 921600`
3. `Tools > CPU Frequency > 240MHz (WiFi/BT)`
4. `Tools > Flash Frequency > 80MHz`
5. `Tools > Flash Mode > QIO`
6. `Tools > Flash Size > 4MB (32Mb)`
7. `Tools > Partition Scheme > Default 4MB with spiffs`
8. `Tools > Core Debug Level > None`
9. `Tools > PSRAM > Enabled`

### 2. Pin Mapping

#### ESP32 DevKit
```cpp
#define BUTTON_UP_PIN 0
#define BUTTON_DOWN_PIN 35
#define BUTTON_LEFT_PIN 34
#define BUTTON_RIGHT_PIN 39
#define BUTTON_SELECT_PIN 36
#define BUTTON_BACK_PIN 32
#define LED_PIN 2
#define BUZZER_PIN 4
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22
#define SD_CS_PIN 15
#define RF_CS_PIN 14
#define RFID_SDA_PIN 21
#define RFID_SCL_PIN 22
#define RFID_IRQ_PIN 25
#define RFID_RST_PIN 26
```

#### M5StickC Plus 2
```cpp
#define BUTTON_A_PIN 37
#define BUTTON_B_PIN 39
#define BUTTON_C_PIN 0
#define LED_PIN 2
#define BUZZER_PIN 2
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22
```

## 🚀 How to Use

### 1. Open the Project
1. Open Arduino IDE
2. `File > Open`
3. Navigate to `EAGLE-FIRMWARE/arduino/EagleFirmware.ino`
4. Open the file

### 2. Configure the Board
1. Select the proper board (ESP32 DevKit or M5StickC Plus 2)
2. Set the options as described above
3. Select the correct COM port

### 3. Compile and Upload
1. Click `Verify` (✓) to compile
2. If no errors, click `Upload` (→)
3. Wait for upload to complete

### 4. Serial Monitor
1. `Tools > Serial Monitor`
2. Set to 115200 baud
3. Observe the boot messages

## 🎮 Controls

### ESP32 DevKit
- **Up Button**: Navigate up in the menu
- **Down Button**: Navigate down in the menu
- **Select Button**: Select/Confirm
- **Back Button**: Go back/Cancel

### M5StickC Plus 2
- **A Button**: Navigate menu
- **B Button**: Select/Confirm
- **C Button**: Go back/Cancel

## 🌐 Web Interface

### Access
1. Connect to WiFi "EAGLE-AP"
2. Open your browser at `192.168.4.1`
3. Use the web interface to control the device

### Web Features
- **WiFi Attacks**: Scan, Beacon Spam, Deauth
- **BLE Attacks**: Scan, Spam, Bad BLE
- **RF Attacks**: Scan, Jammer, Spectrum
- **Status**: Device info

## 📱 Main Menu

### WiFi Attacks
- WiFi Scan
- Beacon Spam
- Deauth Attack
- Evil Portal
- Wardriving
- Raw Sniffer

### BLE Attacks
- BLE Scan
- Bad BLE
- BLE Spam
- BLE Keyboard

### RF Attacks
- RF Scan
- RF Copy
- RF Jammer
- Spectrum
- Custom SubGhz

### RFID Attacks
- Read Tag
- Clone Tag
- Write Tag
- Emulate Tag
- Amiibolink
- Chameleon

### IR Attacks
- TV-B-Gone
- IR Receiver
- IR Transmitter
- Protocols

### FM Attacks
- FM Broadcast
- FM Spectrum
- Traffic Hijack

### NRF24 Attacks
- NRF24 Jammer
- 2.4G Spectrum
- Mousejack

### Scripts
- JavaScript Interpreter
- BadUSB

### ESP-NOW
- Send File
- Receive File
- Send Command

### Utilities
- Mic Spectrum
- QR Codes
- SD Manager
- LittleFS
- RTC/NTP
- Web UI
- PIX System

### Config
- WiFi Config
- Display Config
- Audio Config

## 🔧 Troubleshooting

### Compilation Errors
1. **Library not found**: Install it via Library Manager
2. **Definition error**: Make sure the correct board is selected
3. **Memory error**: Adjust partition settings

### Upload Errors
1. **Port not found**: Check if the USB cable is connected
2. **Upload error**: Press RESET during upload
3. **Verification error**: Check board settings

### Operation Issues
1. **Display not working**: Check pin connections
2. **Buttons not responding**: Check pull-up settings
3. **WiFi not connecting**: Check credentials and signal

### Debug
1. **Serial Monitor**: Use to view debug messages
2. **Logs**: Observe boot logs
3. **Status**: Check device status

## 📊 Monitoring

### Serial Monitor
- Boot messages
- Module status
- Errors and warnings
- Activity logs

### Web Interface
- Real-time status
- Remote control
- Settings
- Activity logs

### Display
- Main menu
- Current status
- Device info
- Navigation

## ⚠️ Important Notices

### Ethical Use
- Use only on your own networks
- Obtain permission before testing
- Follow local laws
- Use for educational purposes only

### Security
- Keep firmware updated
- Use strong passwords
- Monitor activity
- Backup settings

### Hardware
- Check connections
- Use proper power supply
- Protect against short circuit
- Keep device clean

## 📞 Support

### Common Problems
1. **Compilation fails**: Check the libraries
2. **Upload fails**: Check port and settings
3. **Irregular operation**: Check all connections

### Resources
- GitHub Issues: [Link to issues]
- Documentation: [Link to docs]
- Community: [Link to community]

### Contributing
- Report bugs
- Suggest improvements
- Contribute code
- Share your experience

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](../LICENSE) file for details.

---

**Developed with ❤️ for the security community**