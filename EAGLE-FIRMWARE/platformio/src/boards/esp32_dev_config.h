#pragma once

// ESP32 DevKit Configuration
#define BOARD_NAME "ESP32 DevKit"
#define BOARD_TYPE "esp32dev"

// Display Configuration
#define DISPLAY_TYPE "TFT"
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240
#define DISPLAY_CS_PIN 5
#define DISPLAY_DC_PIN 16
#define DISPLAY_RST_PIN 17
#define DISPLAY_MOSI_PIN 23
#define DISPLAY_SCLK_PIN 18
#define DISPLAY_MISO_PIN 19

// Button Configuration
#define BUTTON_UP_PIN 0
#define BUTTON_DOWN_PIN 35
#define BUTTON_LEFT_PIN 34
#define BUTTON_RIGHT_PIN 39
#define BUTTON_SELECT_PIN 36
#define BUTTON_BACK_PIN 32

// LED Configuration
#define LED_PIN 2
#define LED_COUNT 1

// Buzzer Configuration
#define BUZZER_PIN 4

// I2C Configuration
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

// SPI Configuration
#define SPI_MOSI_PIN 23
#define SPI_MISO_PIN 19
#define SPI_SCLK_PIN 18
#define SPI_CS_PIN 5

// SD Card Configuration
#define SD_CS_PIN 15
#define SD_MOSI_PIN 23
#define SD_MISO_PIN 19
#define SD_SCLK_PIN 18

// RF Module Configuration (CC1101)
#define RF_CS_PIN 14
#define RF_MOSI_PIN 23
#define RF_MISO_PIN 19
#define RF_SCLK_PIN 18
#define RF_GDO0_PIN 2
#define RF_GDO2_PIN 3

// RFID Configuration (PN532)
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
#define WIFI_AP_SSID "EAGLE-AP"
#define WIFI_AP_PASSWORD ""
#define WIFI_AP_IP "192.168.4.1"

// Bluetooth Configuration
#define BLE_DEVICE_NAME "EAGLE-ESP32"
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
#define SD_CARD_SIZE 0x8000000

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
