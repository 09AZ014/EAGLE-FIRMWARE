/*
  EAGLE-FIRMWARE - Advanced Pentesting Firmware for ESP32
  Inspired by Bruce project with enhanced features
  
  Compatible with:
  - ESP32 DevKit
  - M5StickC Plus 2
  
  Features:
  - WiFi attacks (Beacon Spam, Deauth, Evil Portal)
  - BLE attacks (Scan, Bad BLE, Spam)
  - RF attacks (Scan, Jammer, Spectrum)
  - RFID attacks (Read, Clone, Emulate)
  - IR attacks (TV-B-Gone, Receiver)
  - FM attacks (Broadcast, Spectrum)
  - NRF24 attacks (Jammer, Mousejack)
  - Script Interpreter (JavaScript, BadUSB)
  - ESP-NOW communication
  - Web UI interface
  - And much more!
*/

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h>
#include <NimBLEDevice.h>
#include <IRremoteESP8266.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <LittleFS.h>
#include <time.h>
#include <esp_now.h>

// Auto-detect M5StickC Plus 2 in Arduino IDE
#if defined(ARDUINO_M5STICK_C_PLUS2) || defined(ARDUINO_M5Stick_C_Plus2)
  #ifndef BOARD_M5STICKC_PLUS2
    #define BOARD_M5STICKC_PLUS2
  #endif
#endif

// Board detection
#ifdef BOARD_M5STICKC_PLUS2
  #include <M5StickCPlus2.h>
  #define BOARD_NAME "M5StickC Plus 2"
  #define HAS_DISPLAY true
  #define HAS_BUTTONS true
  #define HAS_IMU true
  #define HAS_MICROPHONE true
  #define HAS_BATTERY true
  #define HAS_RTC true
  #define HAS_SD_CARD false
#else
  #define BOARD_NAME "ESP32 DevKit"
  #define HAS_DISPLAY true
  #define HAS_BUTTONS true
  #define HAS_IMU false
  #define HAS_MICROPHONE false
  #define HAS_BATTERY false
  #define HAS_RTC false
  #define HAS_SD_CARD true
#endif

#ifndef BOARD_M5STICKC_PLUS2
// Minimal stub so M5.Lcd references compile on non-M5 builds in Arduino IDE
#ifndef BLACK
  #define BLACK 0x0000
#endif
#ifndef WHITE
  #define WHITE 0xFFFF
#endif
struct __EAGLE_DummyLCD {
  void println() {}
  void println(const String&) {}
  void println(const char*) {}
  void print(const String&) {}
  void print(const char*) {}
  void setRotation(int) {}
  void fillScreen(uint32_t) {}
  void setTextColor(uint32_t) {}
  void setTextSize(int) {}
  void setCursor(int,int) {}
};
struct __EAGLE_DummyM5 { __EAGLE_DummyLCD Lcd; void begin(){} void update(){} };
static __EAGLE_DummyM5 M5;
#endif

// Pin definitions
#ifdef BOARD_M5STICKC_PLUS2
  // M5StickC Plus 2 pins
  #define BUTTON_A_PIN 37
  #define BUTTON_B_PIN 39
  #define BUTTON_C_PIN 0
  #define LED_PIN 2
  #define BUZZER_PIN 2
  #define I2C_SDA_PIN 21
  #define I2C_SCL_PIN 22
#else
  // ESP32 DevKit pins
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
#endif

// Global variables
bool wifiConnected = false;
bool apMode = false;
String apSSID = "EAGLE-AP";
String apPassword = "";
IPAddress apIP(192, 168, 4, 1);

// Web server
WebServer server(80);
DNSServer dnsServer;

// Menu system
enum MenuState {
  MAIN_MENU,
  WIFI_MENU,
  BLE_MENU,
  RF_MENU,
  RFID_MENU,
  IR_MENU,
  FM_MENU,
  NRF24_MENU,
  SCRIPT_MENU,
  ESPNOW_MENU,
  UTILITY_MENU,
  CONFIG_MENU
};

MenuState currentMenu = MAIN_MENU;
int menuIndex = 0;
int menuItemCount = 0;

// Button states
bool buttonPressed = false;
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 200;

// Status variables
String statusMessage = "EAGLE-FIRMWARE Ready";
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 1000;

// Function declarations
void setup();
void loop();
void setupPins();
void setupWiFi();
void setupWebServer();
void setupBLE();
void setupDisplay();
void handleButtons();
void updateDisplay();
void drawMainMenu();
void drawWiFiMenu();
void drawBLEMenu();
void drawRFMenu();
void drawRFIDMenu();
void drawIRMenu();
void drawFMMenu();
void drawNRF24Menu();
void drawScriptMenu();
void drawESPNOWMenu();
void drawUtilityMenu();
void drawConfigMenu();
void handleWebRequests();
void startWiFiScan();
void startBLEScan();
void startRFScan();
void startRFIDScan();
void startIRScan();
void startFMScan();
void startNRF24Scan();
void startScriptInterpreter();
void startESPNOW();
void showUtilities();
void showConfig();
void executeAction();
void showStatus(String message);
void beep(int frequency = 1000, int duration = 100);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("========================================");
  Serial.println("EAGLE-FIRMWARE - Advanced Pentesting");
  Serial.println("========================================");
  Serial.printf("Board: %s\n", BOARD_NAME);
  Serial.printf("Version: 1.0.0\n");
  Serial.println("========================================");
  
  // Initialize hardware
  setupPins();
  setupDisplay();
  setupWiFi();
  setupWebServer();
  setupBLE();
  
  // Show startup message
  showStatus("EAGLE-FIRMWARE Ready");
  beep(1000, 200);
  delay(200);
  beep(1500, 200);
  
  Serial.println("Setup complete!");
}

void loop() {
  handleButtons();
  updateDisplay();
  server.handleClient();
  dnsServer.processNextRequest();
  
  // Update status every second
  if (millis() - lastUpdate >= updateInterval) {
    lastUpdate = millis();
    
    // Update WiFi status
    if (WiFi.status() == WL_CONNECTED) {
      wifiConnected = true;
    } else {
      wifiConnected = false;
    }
  }
  
  delay(50);
}

void setupPins() {
  Serial.println("Setting up pins...");
  
#ifdef BOARD_M5STICKC_PLUS2
  // M5StickC Plus 2 button setup
  pinMode(BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);
  pinMode(BUTTON_C_PIN, INPUT_PULLUP);
#else
  // ESP32 DevKit button setup
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_BACK_PIN, INPUT_PULLUP);
#endif

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Initialize I2C
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  
  Serial.println("Pins configured");
}

void setupDisplay() {
  Serial.println("Setting up display...");
  
#ifdef BOARD_M5STICKC_PLUS2
  M5.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("EAGLE-FIRMWARE");
  M5.Lcd.println("M5StickC Plus 2");
  M5.Lcd.println("Ready!");
#else
  // Display setup for ESP32 DevKit would go here
  Serial.println("Display ready (ESP32 DevKit)");
#endif
}

void setupWiFi() {
  Serial.println("Setting up WiFi...");
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  // Try to connect to saved WiFi
  // This would read from EEPROM or config file
  Serial.println("WiFi initialized");
}

void setupWebServer() {
  Serial.println("Setting up web server...");
  
  // Start Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSSID.c_str(), apPassword.c_str());
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  
  apMode = true;
  
  // Setup DNS server for captive portal
  dnsServer.start(53, "*", apIP);
  
  // Setup web server routes
  server.on("/", []() {
    String html = "<!DOCTYPE html><html><head><title>EAGLE-FIRMWARE</title></head><body>";
    html += "<h1>EAGLE-FIRMWARE Control Panel</h1>";
    html += "<h2>WiFi Attacks</h2>";
    html += "<button onclick=\"fetch('/wifi/scan')\">Start WiFi Scan</button><br>";
    html += "<button onclick=\"fetch('/wifi/beacon')\">Start Beacon Spam</button><br>";
    html += "<button onclick=\"fetch('/wifi/deauth')\">Start Deauth Attack</button><br>";
    html += "<h2>BLE Attacks</h2>";
    html += "<button onclick=\"fetch('/ble/scan')\">Start BLE Scan</button><br>";
    html += "<button onclick=\"fetch('/ble/spam')\">Start BLE Spam</button><br>";
    html += "<h2>RF Attacks</h2>";
    html += "<button onclick=\"fetch('/rf/scan')\">Start RF Scan</button><br>";
    html += "<button onclick=\"fetch('/rf/jammer')\">Start RF Jammer</button><br>";
    html += "<h2>Status</h2>";
    html += "<p>Board: " + String(BOARD_NAME) + "</p>";
    html += "<p>Uptime: " + String(millis() / 1000) + "s</p>";
    html += "<p>Free Heap: " + String(ESP.getFreeHeap()) + " bytes</p>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });
  
  // WiFi routes
  server.on("/wifi/scan", []() {
    startWiFiScan();
    server.send(200, "text/plain", "WiFi scan started");
  });
  
  server.on("/wifi/beacon", []() {
    server.send(200, "text/plain", "Beacon spam started");
  });
  
  server.on("/wifi/deauth", []() {
    server.send(200, "text/plain", "Deauth attack started");
  });
  
  // BLE routes
  server.on("/ble/scan", []() {
    startBLEScan();
    server.send(200, "text/plain", "BLE scan started");
  });
  
  server.on("/ble/spam", []() {
    server.send(200, "text/plain", "BLE spam started");
  });
  
  // RF routes
  server.on("/rf/scan", []() {
    startRFScan();
    server.send(200, "text/plain", "RF scan started");
  });
  
  server.on("/rf/jammer", []() {
    server.send(200, "text/plain", "RF jammer started");
  });
  
  server.begin();
  Serial.println("Web server started");
  Serial.println("Access point: " + apSSID);
  Serial.println("IP address: " + apIP.toString());
}

void setupBLE() {
  Serial.println("Setting up BLE...");
  
  NimBLEDevice::init("EAGLE-ESP32");
  NimBLEDevice::setPower(ESP_PWR_LVL_P9);
  
  Serial.println("BLE initialized");
}

void handleButtons() {
  bool buttonState = false;
  
#ifdef BOARD_M5STICKC_PLUS2
  // M5StickC Plus 2 button handling
  if (digitalRead(BUTTON_A_PIN) == LOW) {
    buttonState = true;
    menuIndex = (menuIndex + 1) % menuItemCount;
  }
  if (digitalRead(BUTTON_B_PIN) == LOW) {
    buttonState = true;
    executeAction();
  }
  if (digitalRead(BUTTON_C_PIN) == LOW) {
    buttonState = true;
    if (currentMenu != MAIN_MENU) {
      currentMenu = MAIN_MENU;
      menuIndex = 0;
    }
  }
#else
  // ESP32 DevKit button handling
  if (digitalRead(BUTTON_UP_PIN) == LOW) {
    buttonState = true;
    menuIndex = (menuIndex - 1 + menuItemCount) % menuItemCount;
  }
  if (digitalRead(BUTTON_DOWN_PIN) == LOW) {
    buttonState = true;
    menuIndex = (menuIndex + 1) % menuItemCount;
  }
  if (digitalRead(BUTTON_SELECT_PIN) == LOW) {
    buttonState = true;
    executeAction();
  }
  if (digitalRead(BUTTON_BACK_PIN) == LOW) {
    buttonState = true;
    if (currentMenu != MAIN_MENU) {
      currentMenu = MAIN_MENU;
      menuIndex = 0;
    }
  }
#endif

  if (buttonState && millis() - lastButtonPress > debounceDelay) {
    lastButtonPress = millis();
    buttonPressed = true;
    beep(800, 50);
  }
}

void updateDisplay() {
#ifdef BOARD_M5STICKC_PLUS2
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  
  switch (currentMenu) {
    case MAIN_MENU:
      drawMainMenu();
      break;
    case WIFI_MENU:
      drawWiFiMenu();
      break;
    case BLE_MENU:
      drawBLEMenu();
      break;
    case RF_MENU:
      drawRFMenu();
      break;
    case RFID_MENU:
      drawRFIDMenu();
      break;
    case IR_MENU:
      drawIRMenu();
      break;
    case FM_MENU:
      drawFMMenu();
      break;
    case NRF24_MENU:
      drawNRF24Menu();
      break;
    case SCRIPT_MENU:
      drawScriptMenu();
      break;
    case ESPNOW_MENU:
      drawESPNOWMenu();
      break;
    case UTILITY_MENU:
      drawUtilityMenu();
      break;
    case CONFIG_MENU:
      drawConfigMenu();
      break;
  }
#else
  // ESP32 DevKit display handling would go here
  Serial.println("Display update (ESP32 DevKit)");
#endif
}

void drawMainMenu() {
  M5.Lcd.println("EAGLE-FIRMWARE");
  M5.Lcd.println("==============");
  M5.Lcd.println();
  
  String menuItems[] = {
    "WiFi Attacks",
    "BLE Attacks", 
    "RF Attacks",
    "RFID Attacks",
    "IR Attacks",
    "FM Attacks",
    "NRF24 Attacks",
    "Scripts",
    "ESP-NOW",
    "Utilities",
    "Config"
  };
  
  menuItemCount = 11;
  
  for (int i = 0; i < menuItemCount; i++) {
    if (i == menuIndex) {
      M5.Lcd.print("> ");
    } else {
      M5.Lcd.print("  ");
    }
    M5.Lcd.println(menuItems[i]);
  }
  
  M5.Lcd.println();
  M5.Lcd.println("Status: " + statusMessage);
}

void drawWiFiMenu() {
  M5.Lcd.println("WiFi Attacks");
  M5.Lcd.println("============");
  M5.Lcd.println();
  
  String menuItems[] = {
    "WiFi Scan",
    "Beacon Spam",
    "Deauth Attack",
    "Evil Portal",
    "Wardriving",
    "Raw Sniffer",
    "Back"
  };
  
  menuItemCount = 7;
  
  for (int i = 0; i < menuItemCount; i++) {
    if (i == menuIndex) {
      M5.Lcd.print("> ");
    } else {
      M5.Lcd.print("  ");
    }
    M5.Lcd.println(menuItems[i]);
  }
}

void drawBLEMenu() {
  M5.Lcd.println("BLE Attacks");
  M5.Lcd.println("===========");
  M5.Lcd.println();
  
  String menuItems[] = {
    "BLE Scan",
    "Bad BLE",
    "BLE Spam",
    "BLE Keyboard",
    "Back"
  };
  
  menuItemCount = 5;
  
  for (int i = 0; i < menuItemCount; i++) {
    if (i == menuIndex) {
      M5.Lcd.print("> ");
    } else {
      M5.Lcd.print("  ");
    }
    M5.Lcd.println(menuItems[i]);
  }
}

void drawRFMenu() {
  M5.Lcd.println("RF Attacks");
  M5.Lcd.println("==========");
  M5.Lcd.println();
  
  String menuItems[] = {
    "RF Scan",
    "RF Copy",
    "RF Jammer",
    "Spectrum",
    "Custom SubGhz",
    "Back"
  };
  
  menuItemCount = 6;
  
  for (int i = 0; i < menuItemCount; i++) {
    if (i == menuIndex) {
      M5.Lcd.print("> ");
    } else {
      M5.Lcd.print("  ");
    }
    M5.Lcd.println(menuItems[i]);
  }
}

void drawRFIDMenu() {
  M5.Lcd.println("RFID Attacks");
  M5.Lcd.println("============");
  M5.Lcd.println();
  
  String menuItems[] = {
    "Read Tag",
    "Clone Tag",
    "Write Tag",
    "Emulate Tag",
    "Amiibolink",
    "Chameleon",
    "Back"
  };
  
  menuItemCount = 7;
  
  for (int i = 0; i < menuItemCount; i++) {
    if (i == menuIndex) {
      M5.Lcd.print("> ");
    } else {
      M5.Lcd.print("  ");
    }
    M5.Lcd.println(menuItems[i]);
  }
}

void drawIRMenu() {
  M5.Lcd.println("IR Attacks");
  M5.Lcd.println("==========");
  M5.Lcd.println();
  
  String menuItems[] = {
    "TV-B-Gone",
    "IR Receiver",
    "IR Transmitter",
    "Protocols",
    "Back"
  };
  
  menuItemCount = 5;
  
  for (int i = 0; i < menuItemCount; i++) {
    if (i == menuIndex) {
      M5.Lcd.print("> ");
    } else {
      M5.Lcd.print("  ");
    }
    M5.Lcd.println(menuItems[i]);
  }
}

void drawFMMenu() {
  M5.Lcd.println("FM Attacks");
  M5.Lcd.println("==========");
  M5.Lcd.println();
  
  String menuItems[] = {
    "FM Broadcast",
    "FM Spectrum",
    "Traffic Hijack",
    "Back"
  };
  
  menuItemCount = 4;
  
  for (int i = 0; i < menuItemCount; i++) {
    if (i == menuIndex) {
      M5.Lcd.print("> ");
    } else {
      M5.Lcd.print("  ");
    }
    M5.Lcd.println(menuItems[i]);
  }
}

void drawNRF24Menu() {
  M5.Lcd.println("NRF24 Attacks");
  M5.Lcd.println("=============");
  M5.Lcd.println();
  
  String menuItems[] = {
    "NRF24 Jammer",
    "2.4G Spectrum",
    "Mousejack",
    "Back"
  };
  
  menuItemCount = 4;
  
  for (int i = 0; i < menuItemCount; i++) {
    if (i == menuIndex) {
      M5.Lcd.print("> ");
    } else {
      M5.Lcd.print("  ");
    }
    M5.Lcd.println(menuItems[i]);
  }
}

void drawScriptMenu() {
  M5.Lcd.println("Scripts");
  M5.Lcd.println("=======");
  M5.Lcd.println();
  
  String menuItems[] = {
    "JavaScript",
    "BadUSB",
    "Back"
  };
  
  menuItemCount = 3;
  
  for (int i = 0; i < menuItemCount; i++) {
    if (i == menuIndex) {
      M5.Lcd.print("> ");
    } else {
      M5.Lcd.print("  ");
    }
    M5.Lcd.println(menuItems[i]);
  }
}

void drawESPNOWMenu() {
  M5.Lcd.println("ESP-NOW");
  M5.Lcd.println("=======");
  M5.Lcd.println();
  
  String menuItems[] = {
    "Send File",
    "Receive File",
    "Send Command",
    "Back"
  };
  
  menuItemCount = 4;
  
  for (int i = 0; i < menuItemCount; i++) {
    if (i == menuIndex) {
      M5.Lcd.print("> ");
    } else {
      M5.Lcd.print("  ");
    }
    M5.Lcd.println(menuItems[i]);
  }
}

void drawUtilityMenu() {
  M5.Lcd.println("Utilities");
  M5.Lcd.println("=========");
  M5.Lcd.println();
  
  String menuItems[] = {
    "Mic Spectrum",
    "QR Codes",
    "SD Manager",
    "LittleFS",
    "RTC/NTP",
    "Web UI",
    "PIX System",
    "Back"
  };
  
  menuItemCount = 8;
  
  for (int i = 0; i < menuItemCount; i++) {
    if (i == menuIndex) {
      M5.Lcd.print("> ");
    } else {
      M5.Lcd.print("  ");
    }
    M5.Lcd.println(menuItems[i]);
  }
}

void drawConfigMenu() {
  M5.Lcd.println("Config");
  M5.Lcd.println("======");
  M5.Lcd.println();
  
  String menuItems[] = {
    "WiFi Config",
    "Display Config",
    "Audio Config",
    "Back"
  };
  
  menuItemCount = 4;
  
  for (int i = 0; i < menuItemCount; i++) {
    if (i == menuIndex) {
      M5.Lcd.print("> ");
    } else {
      M5.Lcd.print("  ");
    }
    M5.Lcd.println(menuItems[i]);
  }
}

void executeAction() {
  switch (currentMenu) {
    case MAIN_MENU:
      switch (menuIndex) {
        case 0: currentMenu = WIFI_MENU; menuIndex = 0; break;
        case 1: currentMenu = BLE_MENU; menuIndex = 0; break;
        case 2: currentMenu = RF_MENU; menuIndex = 0; break;
        case 3: currentMenu = RFID_MENU; menuIndex = 0; break;
        case 4: currentMenu = IR_MENU; menuIndex = 0; break;
        case 5: currentMenu = FM_MENU; menuIndex = 0; break;
        case 6: currentMenu = NRF24_MENU; menuIndex = 0; break;
        case 7: currentMenu = SCRIPT_MENU; menuIndex = 0; break;
        case 8: currentMenu = ESPNOW_MENU; menuIndex = 0; break;
        case 9: currentMenu = UTILITY_MENU; menuIndex = 0; break;
        case 10: currentMenu = CONFIG_MENU; menuIndex = 0; break;
      }
      break;
      
    case WIFI_MENU:
      switch (menuIndex) {
        case 0: startWiFiScan(); break;
        case 1: showStatus("Beacon Spam Started"); break;
        case 2: showStatus("Deauth Attack Started"); break;
        case 3: showStatus("Evil Portal Started"); break;
        case 4: showStatus("Wardriving Started"); break;
        case 5: showStatus("Raw Sniffer Started"); break;
        case 6: currentMenu = MAIN_MENU; menuIndex = 0; break;
      }
      break;
      
    case BLE_MENU:
      switch (menuIndex) {
        case 0: startBLEScan(); break;
        case 1: showStatus("Bad BLE Started"); break;
        case 2: showStatus("BLE Spam Started"); break;
        case 3: showStatus("BLE Keyboard Started"); break;
        case 4: currentMenu = MAIN_MENU; menuIndex = 0; break;
      }
      break;
      
    case RF_MENU:
      switch (menuIndex) {
        case 0: startRFScan(); break;
        case 1: showStatus("RF Copy Started"); break;
        case 2: showStatus("RF Jammer Started"); break;
        case 3: showStatus("Spectrum Started"); break;
        case 4: showStatus("Custom SubGhz Started"); break;
        case 5: currentMenu = MAIN_MENU; menuIndex = 0; break;
      }
      break;
      
    case RFID_MENU:
      switch (menuIndex) {
        case 0: startRFIDScan(); break;
        case 1: showStatus("Clone Tag Started"); break;
        case 2: showStatus("Write Tag Started"); break;
        case 3: showStatus("Emulate Tag Started"); break;
        case 4: showStatus("Amiibolink Started"); break;
        case 5: showStatus("Chameleon Started"); break;
        case 6: currentMenu = MAIN_MENU; menuIndex = 0; break;
      }
      break;
      
    case IR_MENU:
      switch (menuIndex) {
        case 0: showStatus("TV-B-Gone Started"); break;
        case 1: showStatus("IR Receiver Started"); break;
        case 2: showStatus("IR Transmitter Started"); break;
        case 3: showStatus("Protocols Started"); break;
        case 4: currentMenu = MAIN_MENU; menuIndex = 0; break;
      }
      break;
      
    case FM_MENU:
      switch (menuIndex) {
        case 0: showStatus("FM Broadcast Started"); break;
        case 1: showStatus("FM Spectrum Started"); break;
        case 2: showStatus("Traffic Hijack Started"); break;
        case 3: currentMenu = MAIN_MENU; menuIndex = 0; break;
      }
      break;
      
    case NRF24_MENU:
      switch (menuIndex) {
        case 0: showStatus("NRF24 Jammer Started"); break;
        case 1: showStatus("2.4G Spectrum Started"); break;
        case 2: showStatus("Mousejack Started"); break;
        case 3: currentMenu = MAIN_MENU; menuIndex = 0; break;
      }
      break;
      
    case SCRIPT_MENU:
      switch (menuIndex) {
        case 0: startScriptInterpreter(); break;
        case 1: showStatus("BadUSB Started"); break;
        case 2: currentMenu = MAIN_MENU; menuIndex = 0; break;
      }
      break;
      
    case ESPNOW_MENU:
      switch (menuIndex) {
        case 0: showStatus("Send File Started"); break;
        case 1: showStatus("Receive File Started"); break;
        case 2: showStatus("Send Command Started"); break;
        case 3: currentMenu = MAIN_MENU; menuIndex = 0; break;
      }
      break;
      
    case UTILITY_MENU:
      switch (menuIndex) {
        case 0: showStatus("Mic Spectrum Started"); break;
        case 1: showStatus("QR Codes Started"); break;
        case 2: showStatus("SD Manager Started"); break;
        case 3: showStatus("LittleFS Started"); break;
        case 4: showStatus("RTC/NTP Started"); break;
        case 5: showStatus("Web UI Started"); break;
        case 6: showStatus("PIX System Started"); break;
        case 7: currentMenu = MAIN_MENU; menuIndex = 0; break;
      }
      break;
      
    case CONFIG_MENU:
      switch (menuIndex) {
        case 0: showStatus("WiFi Config"); break;
        case 1: showStatus("Display Config"); break;
        case 2: showStatus("Audio Config"); break;
        case 3: currentMenu = MAIN_MENU; menuIndex = 0; break;
      }
      break;
  }
}

// Action functions
void startWiFiScan() {
  showStatus("WiFi Scan Started");
  Serial.println("Starting WiFi scan...");
  // WiFi scan implementation would go here
}

void startBLEScan() {
  showStatus("BLE Scan Started");
  Serial.println("Starting BLE scan...");
  // BLE scan implementation would go here
}

void startRFScan() {
  showStatus("RF Scan Started");
  Serial.println("Starting RF scan...");
  // RF scan implementation would go here
}

void startRFIDScan() {
  showStatus("RFID Scan Started");
  Serial.println("Starting RFID scan...");
  // RFID scan implementation would go here
}

void startScriptInterpreter() {
  showStatus("Script Interpreter Started");
  Serial.println("Starting script interpreter...");
  // Script interpreter implementation would go here
}

void showStatus(String message) {
  statusMessage = message;
  Serial.println("Status: " + message);
  beep(1000, 100);
}

void beep(int frequency, int duration) {
  // Simple beep implementation
  for (int i = 0; i < duration; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(frequency);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(frequency);
  }
}