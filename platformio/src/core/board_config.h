#pragma once

// Board detection and configuration
#ifdef BOARD_M5STICKC_PLUS2
    #include "../boards/m5stickc_plus2.h"
    #define EAGLE_BOARD_NAME BoardM5StickC2::NAME
#else
    #include "../boards/devkit_esp32.h"
    #define EAGLE_BOARD_NAME BoardDevKit::NAME
#endif

// Feature flags
#ifdef EAGLE_BRUCE_FEATURES
    #define ENABLE_WIFI_ATTACKS
    #define ENABLE_BLE_ATTACKS
    #define ENABLE_RF_ATTACKS
    #define ENABLE_RFID_ATTACKS
    #define ENABLE_IR_ATTACKS
    #define ENABLE_FM_ATTACKS
    #define ENABLE_NRF24_ATTACKS
    #define ENABLE_SCRIPT_INTERPRETER
    #define ENABLE_BADUSB
    #define ENABLE_WARDIVING
    #define ENABLE_EVIL_PORTAL
    #define ENABLE_RAW_SNIFFER
    #define ENABLE_ESP_NOW
#endif

// Board-specific feature availability
#ifdef BOARD_M5STICKC_PLUS2
    #define HAS_DISPLAY true
    #define HAS_BUTTONS true
    #define HAS_LED true
    #define HAS_BUZZER true
    #define HAS_IMU true
    #define HAS_MICROPHONE true
    #define HAS_BATTERY true
    #define HAS_RTC true
    #define HAS_GROVE_PORT true
    #define HAS_SD_CARD false
#else
    #define HAS_DISPLAY true
    #define HAS_BUTTONS true
    #define HAS_LED true
    #define HAS_BUZZER true
    #define HAS_IMU false
    #define HAS_MICROPHONE false
    #define HAS_BATTERY false
    #define HAS_RTC false
    #define HAS_GROVE_PORT false
    #define HAS_SD_CARD true
#endif

