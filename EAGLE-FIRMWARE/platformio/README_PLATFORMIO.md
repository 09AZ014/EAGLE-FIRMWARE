# EAGLE-FIRMWARE – Build and Upload with PlatformIO

## Supported Environments
- **esp32dev** (Standard ESP32)
- **m5stickc_plus2** (M5 Stick C Plus 2)

---

## Prerequisites
- **Python 3.x** installed
- **PlatformIO CLI** (install via `pip install platformio` or `pipx install platformio`)
- **USB/Serial Drivers** for your board (e.g., CP210x, CH340, etc.)
- Data USB cable (not just charging)

---

## Firmware Compilation
1. **Enter the PlatformIO folder:**
   ```bash
   cd EAGLE-FIRMWARE/platformio
   ```
2. **Compile for standard ESP32:**
   ```bash
   python -m platformio run -e esp32dev
   ```
3. **Compile for M5 Stick C Plus 2:**
   ```bash
   python -m platformio run -e m5stickc_plus2
   ```
---

## Firmware Upload (Flashing)
### 1. Make sure your board is properly connected via USB.
### 2. Find your serial port (on Windows, it’s usually COM3, COM4, etc.; on Linux/Mac, /dev/ttyUSB0 or /dev/ttyACM0).

#### Standard ESP32:
```bash
python -m platformio run -t upload -e esp32dev
# or specify the port:
python -m platformio run -t upload -e esp32dev --upload-port COM4
```

#### M5 Stick C Plus 2:
```bash
python -m platformio run -t upload -e m5stickc_plus2
# or specify the port:
python -m platformio run -t upload -e m5stickc_plus2 --upload-port COM4
```

---

## Filesystem Upload (LittleFS or SPIFFS)
If your firmware uses web files or saved settings in LittleFS, run:
```bash
python -m platformio run -t uploadfs -e esp32dev
python -m platformio run -t uploadfs -e m5stickc_plus2
```

---

## Serial Monitor
For logs, messages, or interacting with the firmware:
```bash
python -m platformio device monitor -e esp32dev
# or
python -m platformio device monitor --port COM4 --baud 115200
```
> Tip: always use the same baud rate as set in `platformio.ini` (usually 115200).

---

## Troubleshooting
- **Port permissions (Linux/Mac):**
  - Add your user to the `dialout` group and restart if needed.
- **Driver problems:**
  - Use Device Manager (Windows) or `dmesg | grep tty` (Linux) to check if the board was recognized.
- **Permission denied:**
  - Run the terminal as administrator (Windows) or use `sudo` (Linux – but avoid it if possible).
- **PATH error after installing PlatformIO:**
  - Always try running using `python -m platformio ...` to guarantee the correct local scripts are used.

---

## Project Structure
- `src/core/` – core files (configuration, module management)
- `src/modules/` – main modules (Wi-Fi, OTA, REST API, IMU, Display, etc.)
- `src/boards/` – board-specific settings
- `data/` – files for LittleFS (Mini Web UI)
- `platformio.ini` – environment, dependency and flag configuration

---

## Questions?
Open an issue or check the main README.

