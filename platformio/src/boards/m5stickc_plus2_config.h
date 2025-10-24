#pragma once

// M5StickC Plus 2 Configuration
#define BOARD_NAME "M5StickC Plus 2"
#define BOARD_TYPE "m5stickc_plus2"

// Display Configuration (ST7789)
#define DISPLAY_TYPE "ST7789"
#define DISPLAY_WIDTH 135
#define DISPLAY_HEIGHT 240
#define DISPLAY_CS_PIN 5
#define DISPLAY_DC_PIN 23
#define DISPLAY_RST_PIN 18
#define DISPLAY_MOSI_PIN 15
#define DISPLAY_SCLK_PIN 13
#define DISPLAY_MISO_PIN 12

// Button Configuration
#define BUTTON_A_PIN 37
#define BUTTON_B_PIN 39
#define BUTTON_C_PIN 0

// LED Configuration
#define LED_PIN 2
#define LED_COUNT 1

// Buzzer Configuration
#define BUZZER_PIN 2

// I2C Configuration
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

// SPI Configuration
#define SPI_MOSI_PIN 15
#define SPI_MISO_PIN 12
#define SPI_SCLK_PIN 13
#define SPI_CS_PIN 5

// SD Card Configuration (Not available on M5StickC Plus 2)
#define SD_CS_PIN -1
#define SD_MOSI_PIN -1
#define SD_MISO_PIN -1
#define SD_SCLK_PIN -1

// RF Module Configuration (CC1101) - External module
#define RF_CS_PIN 14
#define RF_MOSI_PIN 15
#define RF_MISO_PIN 12
#define RF_SCLK_PIN 13
#define RF_GDO0_PIN 2
#define RF_GDO2_PIN 3

// RFID Configuration (PN532) - External module
#define RFID_SDA_PIN 21
#define RFID_SCL_PIN 22
#define RFID_IRQ_PIN 25
#define RFID_RST_PIN 26

// IR Configuration
#define IR_TX_PIN 27
#define IR_RX_PIN 26

// Audio Configuration
#define AUDIO_TX_PIN 25
#define AUDIO_RX_PIN 26

// ESP-NOW Configuration
#define ESPNOW_CHANNEL 1
#define ESPNOW_ENCRYPT false

// WiFi Configuration
#define WIFI_AP_SSID "EAGLE-M5"
#define WIFI_AP_PASSWORD ""
#define WIFI_AP_IP "192.168.4.1"

// Bluetooth Configuration
#define BLE_DEVICE_NAME "EAGLE-M5"
#define BLE_ADVERTISING_INTERVAL 100

// RF Configuration
#define RF_FREQUENCY 433920000
#define RF_POWER 7
#define RF_MODULATION 0

// RFID Configuration
#define RFID_TYPE 1 // PN532
#define RFID_BAUD_RATE 115200

// IR Configuration
#define IR_PROTOCOL NEC
#define IR_BITS 32

// Audio Configuration
#define AUDIO_SAMPLE_RATE 44100
#define AUDIO_BITS_PER_SAMPLE 16

// Storage Configuration
#define LITTLEFS_SIZE 0x100000
#define SD_CARD_SIZE 0 // Not available

// Network Configuration
#define MAX_CONNECTIONS 4
#define TCP_PORT 8080
#define UDP_PORT 8080

// Security Configuration
#define API_TOKEN_LENGTH 32
#define MAX_PASSWORD_LENGTH 64

// Performance Configuration
#define TASK_STACK_SIZE 4096
#define TASK_PRIORITY 1
#define WATCHDOG_TIMEOUT 30000

// Debug Configuration
#define DEBUG_LEVEL 1
#define SERIAL_BAUD_RATE 115200

// M5StickC Plus 2 Specific Features
#define HAS_IMU true
#define HAS_MICROPHONE true
#define HAS_BATTERY true
#define HAS_RTC true
#define HAS_GROVE_PORT true

// IMU Configuration
#define IMU_SDA_PIN 21
#define IMU_SCL_PIN 22
#define IMU_ADDRESS 0x68

// Microphone Configuration
#define MIC_PIN 34
#define MIC_SAMPLE_RATE 44100
#define MIC_BITS_PER_SAMPLE 16

// Battery Configuration
#define BATTERY_PIN 35
#define BATTERY_VOLTAGE_DIVIDER 2.0

// RTC Configuration
#define RTC_SDA_PIN 21
#define RTC_SCL_PIN 22
#define RTC_ADDRESS 0x51

// Grove Port Configuration
#define GROVE_SDA_PIN 21
#define GROVE_SCL_PIN 22
#define GROVE_POWER_PIN 26
