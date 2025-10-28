#include "menu_module.h"
#include "wifi_attacks_module.h"
#include "ble_attacks_module.h"
#include "ir_module.h"
#include "evil_portal_module.h"

void MenuModule::setup() {
    Serial.println("[Menu] Initializing menu system...");
    
    initMainMenu();
    initWiFiMenu();
    initBLEMenu();
    initRFMenu();
    initRFIDMenu();
    initIRMenu();
    initBadUSBMenu();
    initOtherMenu();
    initSettingsMenu();
    
    lastButtonPress = 0;
    
    Serial.println("[Menu] Menu system ready!");
    
#ifdef BOARD_M5STICKC_PLUS2
    // Desenhar menu inicial
    drawMenu();
#endif
}

void MenuModule::loop() {
#ifdef BOARD_M5STICKC_PLUS2
    handleButtons();
#endif
}

// ========== INICIALIZAÇÃO DOS MENUS ==========

void MenuModule::initMainMenu() {
    mainMenu.clear();
    mainMenu.push_back({"WiFi", TFT_CYAN, MENU_WIFI, nullptr});
    mainMenu.push_back({"BLE", TFT_BLUE, MENU_BLE, nullptr});
    mainMenu.push_back({"RF/SubGhz", TFT_MAGENTA, MENU_RF, nullptr});
    mainMenu.push_back({"RFID/NFC", TFT_YELLOW, MENU_RFID, nullptr});
    mainMenu.push_back({"Infrared", TFT_RED, MENU_IR, nullptr});
    mainMenu.push_back({"BadUSB", TFT_ORANGE, MENU_BADUSB, nullptr});
    mainMenu.push_back({"Other", TFT_GREEN, MENU_OTHER, nullptr});
    mainMenu.push_back({"Settings", TFT_LIGHTGREY, MENU_SETTINGS, nullptr});
}

void MenuModule::initWiFiMenu() {
    wifiMenu.clear();
    wifiMenu.push_back({"Scan Networks", TFT_CYAN, MENU_WIFI, [this]() { actionWiFiScan(); }});
    wifiMenu.push_back({"Beacon Spam", TFT_YELLOW, MENU_WIFI, [this]() { actionWiFiBeaconSpam(); }});
    wifiMenu.push_back({"Deauth Flood", TFT_RED, MENU_WIFI, [this]() { actionWiFiDeauthFlood(); }});
    wifiMenu.push_back({"Evil Portal", TFT_ORANGE, MENU_WIFI, [this]() { actionWiFiEvilPortal(); }});
    wifiMenu.push_back({"Packet Sniffer", TFT_MAGENTA, MENU_WIFI, [this]() { actionWiFiSniffer(); }});
    wifiMenu.push_back({"< Back", TFT_LIGHTGREY, MENU_MAIN, nullptr});
}

void MenuModule::initBLEMenu() {
    bleMenu.clear();
    bleMenu.push_back({"BLE Scan", TFT_CYAN, MENU_BLE, [this]() { actionBLEScan(); }});
    bleMenu.push_back({"Spam All", TFT_WHITE, MENU_BLE, [this]() { actionBLESpamAll(); }});
    bleMenu.push_back({"iOS Spam", TFT_YELLOW, MENU_BLE, [this]() { actionBLESpamIOS(); }});
    bleMenu.push_back({"Android Spam", TFT_GREEN, MENU_BLE, [this]() { actionBLESpamAndroid(); }});
    bleMenu.push_back({"Samsung Spam", TFT_BLUE, MENU_BLE, [this]() { actionBLESpamSamsung(); }});
    bleMenu.push_back({"Windows Spam", TFT_CYAN, MENU_BLE, [this]() { actionBLESpamWindows(); }});
    bleMenu.push_back({"< Back", TFT_LIGHTGREY, MENU_MAIN, nullptr});
}

void MenuModule::initRFMenu() {
    rfMenu.clear();
    rfMenu.push_back({"RF Scan", TFT_CYAN, MENU_RF, nullptr});
    rfMenu.push_back({"RF Replay", TFT_YELLOW, MENU_RF, nullptr});
    rfMenu.push_back({"RF Jammer", TFT_RED, MENU_RF, nullptr});
    rfMenu.push_back({"[CC1101 Required]", TFT_DARKGREY, MENU_RF, nullptr});
    rfMenu.push_back({"< Back", TFT_LIGHTGREY, MENU_MAIN, nullptr});
}

void MenuModule::initRFIDMenu() {
    rfidMenu.clear();
    rfidMenu.push_back({"Read Tag", TFT_CYAN, MENU_RFID, nullptr});
    rfidMenu.push_back({"Clone Tag", TFT_YELLOW, MENU_RFID, nullptr});
    rfidMenu.push_back({"Emulate Tag", TFT_GREEN, MENU_RFID, nullptr});
    rfidMenu.push_back({"[PN532 Required]", TFT_DARKGREY, MENU_RFID, nullptr});
    rfidMenu.push_back({"< Back", TFT_LIGHTGREY, MENU_MAIN, nullptr});
}

void MenuModule::initIRMenu() {
    irMenu.clear();
    irMenu.push_back({"TV-B-Gone", TFT_RED, MENU_IR, [this]() { actionIRTVBGone(); }});
    irMenu.push_back({"Learn IR", TFT_YELLOW, MENU_IR, [this]() { actionIRLearn(); }});
    irMenu.push_back({"Replay IR", TFT_GREEN, MENU_IR, [this]() { actionIRReplay(); }});
    irMenu.push_back({"< Back", TFT_LIGHTGREY, MENU_MAIN, nullptr});
}

void MenuModule::initBadUSBMenu() {
    badusbMenu.clear();
    badusbMenu.push_back({"Run Script", TFT_YELLOW, MENU_BADUSB, nullptr});
    badusbMenu.push_back({"Script List", TFT_CYAN, MENU_BADUSB, nullptr});
    badusbMenu.push_back({"[USB HID Required]", TFT_DARKGREY, MENU_BADUSB, nullptr});
    badusbMenu.push_back({"< Back", TFT_LIGHTGREY, MENU_MAIN, nullptr});
}

void MenuModule::initOtherMenu() {
    otherMenu.clear();
    otherMenu.push_back({"Stop All", TFT_RED, MENU_OTHER, [this]() { actionStopAll(); }});
    otherMenu.push_back({"System Info", TFT_CYAN, MENU_OTHER, [this]() { actionSystemInfo(); }});
    otherMenu.push_back({"About", TFT_YELLOW, MENU_OTHER, [this]() { actionAbout(); }});
    otherMenu.push_back({"< Back", TFT_LIGHTGREY, MENU_MAIN, nullptr});
}

void MenuModule::initSettingsMenu() {
    settingsMenu.clear();
    settingsMenu.push_back({"Rotate Screen", TFT_CYAN, MENU_SETTINGS, [this]() { rotateScreen(); }});
    settingsMenu.push_back({"Brightness", TFT_YELLOW, MENU_SETTINGS, nullptr});
    settingsMenu.push_back({"WiFi Config", TFT_GREEN, MENU_SETTINGS, nullptr});
    settingsMenu.push_back({"< Back", TFT_LIGHTGREY, MENU_MAIN, nullptr});
}

// ========== NAVEGAÇÃO ==========

void MenuModule::handleButtons() {
#ifdef BOARD_M5STICKC_PLUS2
    M5.update();
    
    unsigned long now = millis();
    if (now - lastButtonPress < debounceDelay) return;
    
    // Botão A (principal) - Navegar para baixo / Selecionar (se mantido)
    if (M5.BtnA.wasPressed()) {
        navigateDown();
        lastButtonPress = now;
    }
    else if (M5.BtnA.pressedFor(500)) {
        selectItem();
        lastButtonPress = now;
    }
    
    // Botão B (power) - Voltar / Navegar para cima (se mantido)
    if (M5.BtnPWR.wasPressed()) {
        goBack();
        lastButtonPress = now;
    }
    else if (M5.BtnPWR.pressedFor(500)) {
        navigateUp();
        lastButtonPress = now;
    }
#endif
}

void MenuModule::navigateUp() {
    auto& menu = getCurrentMenu();
    if (selectedIndex > 0) {
        selectedIndex--;
        drawMenu();
    }
}

void MenuModule::navigateDown() {
    auto& menu = getCurrentMenu();
    if (selectedIndex < menu.size() - 1) {
        selectedIndex++;
        drawMenu();
    }
}

void MenuModule::selectItem() {
    auto& menu = getCurrentMenu();
    if (selectedIndex >= 0 && selectedIndex < menu.size()) {
        MenuItem& item = menu[selectedIndex];
        
        // Se tem ação, executar
        if (item.action) {
            item.action();
        }
        // Se tem próximo estado, mudar
        else if (item.nextState != currentState) {
            currentState = item.nextState;
            selectedIndex = 0;
            drawMenu();
        }
    }
}

void MenuModule::goBack() {
    if (currentState != MENU_MAIN) {
        currentState = MENU_MAIN;
        selectedIndex = 0;
        drawMenu();
    }
}

void MenuModule::rotateScreen() {
#ifdef BOARD_M5STICKC_PLUS2
    rotation = (rotation + 1) % 4;
    M5.Display.setRotation(rotation);
    drawMenu();
    showMessage("Screen rotated", TFT_GREEN, 1000);
#endif
}

// ========== DESENHO ==========

void MenuModule::drawMenu() {
#ifdef BOARD_M5STICKC_PLUS2
    M5.Display.fillScreen(TFT_BLACK);
    
    // Header
    drawHeader(getCurrentMenuTitle());
    
    // Menu items
    auto& menu = getCurrentMenu();
    int startY = 25;
    int itemHeight = 18;
    
    // Calcular scroll se necessário
    int visibleItems = 6;
    int scrollOffset = 0;
    if (selectedIndex >= visibleItems) {
        scrollOffset = selectedIndex - visibleItems + 1;
    }
    
    for (int i = scrollOffset; i < menu.size() && i < scrollOffset + visibleItems; i++) {
        int y = startY + (i - scrollOffset) * itemHeight;
        drawMenuItem(i, menu[i], y);
    }
    
    // Status bar
    drawStatusBar();
#endif
}

void MenuModule::drawHeader(const char* title) {
#ifdef BOARD_M5STICKC_PLUS2
    M5.Display.fillRect(0, 0, 240, 20, TFT_DARKGREY);
    M5.Display.setTextSize(1);
    M5.Display.setTextColor(TFT_YELLOW);
    M5.Display.setCursor(5, 5);
    M5.Display.print("EAGLE");
    
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.setCursor(50, 5);
    M5.Display.print(title);
#endif
}

void MenuModule::drawMenuItem(int index, const MenuItem& item, int y) {
#ifdef BOARD_M5STICKC_PLUS2
    bool isSelected = (index == selectedIndex);
    
    // Background
    if (isSelected) {
        M5.Display.fillRect(0, y, 240, 16, TFT_DARKGREY);
    }
    
    // Seta de seleção
    if (isSelected) {
        M5.Display.setTextColor(TFT_YELLOW);
        M5.Display.setCursor(2, y + 2);
        M5.Display.print(">");
    }
    
    // Item text
    M5.Display.setTextColor(isSelected ? TFT_WHITE : item.color);
    M5.Display.setTextSize(1);
    M5.Display.setCursor(12, y + 2);
    
    // Truncar texto se muito longo
    String displayName = item.name;
    if (displayName.length() > 26) {
        displayName = displayName.substring(0, 23) + "...";
    }
    M5.Display.print(displayName);
#endif
}

void MenuModule::drawStatusBar() {
#ifdef BOARD_M5STICKC_PLUS2
    int statusY = 130;
    M5.Display.fillRect(0, statusY, 240, 10, TFT_BLACK);
    M5.Display.setTextSize(1);
    M5.Display.setTextColor(TFT_DARKGREY);
    M5.Display.setCursor(2, statusY);
    M5.Display.printf("A:Next B:Back | Heap:%dK", ESP.getFreeHeap()/1024);
#endif
}

// ========== AÇÕES WiFi ==========

void MenuModule::actionWiFiScan() {
    showMessage("WiFi Scanning...", TFT_CYAN);
    if (wifiAttacks) {
        auto networks = wifiAttacks->scanNetworks();
        char msg[32];
        sprintf(msg, "Found %d networks", networks.size());
        showMessage(msg, TFT_GREEN);
    }
}

void MenuModule::actionWiFiBeaconSpam() {
    showMessage("Beacon Spam Started", TFT_YELLOW);
    if (wifiAttacks) {
        std::vector<String> ssids = {"Free WiFi", "FBI Surveillance", "Virus.exe", "Test Network"};
        // Executar em task separada para não bloquear UI
        xTaskCreate([](void* param) {
            auto* attacks = (WiFiAttacksModule*)param;
            attacks->startBeaconSpam({"Free WiFi", "FBI Van", "Virus.exe"});
            vTaskDelete(NULL);
        }, "BeaconTask", 4096, wifiAttacks, 1, NULL);
    }
}

void MenuModule::actionWiFiDeauthFlood() {
    showMessage("Deauth Flood Started", TFT_RED);
    if (wifiAttacks) {
        xTaskCreate([](void* param) {
            auto* attacks = (WiFiAttacksModule*)param;
            attacks->startDeauthFlood();
            vTaskDelete(NULL);
        }, "DeauthTask", 4096, wifiAttacks, 1, NULL);
    }
}

void MenuModule::actionWiFiEvilPortal() {
    showMessage("Evil Portal Started", TFT_ORANGE);
    if (evilPortal) {
        evilPortal->startPortal("Free WiFi", "Starbucks WiFi");
    }
}

void MenuModule::actionWiFiSniffer() {
    showMessage("Sniffer - Coming Soon", TFT_DARKGREY);
}

// ========== AÇÕES BLE ==========

void MenuModule::actionBLEScan() {
    showMessage("BLE Scanning...", TFT_CYAN);
    if (bleAttacks) {
        bleAttacks->scanBLEDevices(10);
        showMessage("Scan Complete", TFT_GREEN);
    }
}

void MenuModule::actionBLESpamAll() {
    showMessage("BLE Spam All Started", TFT_WHITE);
    if (bleAttacks) {
        xTaskCreate([](void* param) {
            auto* ble = (BLEAttacksModule*)param;
            ble->startBLESpam(SPAM_ALL);
            vTaskDelete(NULL);
        }, "BLESpamTask", 4096, bleAttacks, 1, NULL);
    }
}

void MenuModule::actionBLESpamIOS() {
    showMessage("iOS Spam Started", TFT_YELLOW);
    if (bleAttacks) {
        xTaskCreate([](void* param) {
            auto* ble = (BLEAttacksModule*)param;
            ble->startAppleJuiceAttack();
            vTaskDelete(NULL);
        }, "iOSSpamTask", 4096, bleAttacks, 1, NULL);
    }
}

void MenuModule::actionBLESpamAndroid() {
    showMessage("Android Spam Started", TFT_GREEN);
    if (bleAttacks) {
        xTaskCreate([](void* param) {
            auto* ble = (BLEAttacksModule*)param;
            ble->startFastPairAttack();
            vTaskDelete(NULL);
        }, "AndroidSpamTask", 4096, bleAttacks, 1, NULL);
    }
}

void MenuModule::actionBLESpamSamsung() {
    showMessage("Samsung Spam Started", TFT_BLUE);
    if (bleAttacks) {
        xTaskCreate([](void* param) {
            auto* ble = (BLEAttacksModule*)param;
            ble->startSamsungSpam();
            vTaskDelete(NULL);
        }, "SamsungSpamTask", 4096, bleAttacks, 1, NULL);
    }
}

void MenuModule::actionBLESpamWindows() {
    showMessage("Windows Spam Started", TFT_CYAN);
    if (bleAttacks) {
        xTaskCreate([](void* param) {
            auto* ble = (BLEAttacksModule*)param;
            ble->startSwiftPairAttack();
            vTaskDelete(NULL);
        }, "WindowsSpamTask", 4096, bleAttacks, 1, NULL);
    }
}

// ========== AÇÕES IR ==========

void MenuModule::actionIRTVBGone() {
    showMessage("TV-B-Gone Running...", TFT_RED);
    if (irModule) {
        irModule->tvbGone();
        showMessage("TV-B-Gone Complete", TFT_GREEN);
    }
}

void MenuModule::actionIRLearn() {
    showMessage("IR Learn - Coming Soon", TFT_DARKGREY);
}

void MenuModule::actionIRReplay() {
    showMessage("IR Replay - Coming Soon", TFT_DARKGREY);
}

// ========== AÇÕES OUTROS ==========

void MenuModule::actionStopAll() {
    showMessage("Stopping All Attacks...", TFT_RED);
    if (wifiAttacks) wifiAttacks->stopAttack();
    if (bleAttacks) bleAttacks->stopSpam();
    if (evilPortal) evilPortal->stopPortal();
    showMessage("All Attacks Stopped", TFT_GREEN);
}

void MenuModule::actionSystemInfo() {
#ifdef BOARD_M5STICKC_PLUS2
    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.setTextSize(1);
    M5.Display.setTextColor(TFT_YELLOW);
    M5.Display.setCursor(5, 5);
    M5.Display.println("=== SYSTEM INFO ===");
    
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.printf("\nBoard: M5StickC Plus 2\n");
    M5.Display.printf("Chip: %s\n", ESP.getChipModel());
    M5.Display.printf("Cores: %d\n", ESP.getChipCores());
    M5.Display.printf("CPU: %d MHz\n", ESP.getCpuFreqMHz());
    M5.Display.printf("Flash: %d MB\n", ESP.getFlashChipSize()/1024/1024);
    M5.Display.printf("Free Heap: %d KB\n", ESP.getFreeHeap()/1024);
    M5.Display.printf("Uptime: %lu s\n", millis()/1000);
    
    M5.Display.setTextColor(TFT_CYAN);
    M5.Display.printf("\nPress B to go back");
    
    while(true) {
        M5.update();
        if (M5.BtnPWR.wasPressed()) break;
        delay(10);
    }
    drawMenu();
#endif
}

void MenuModule::actionAbout() {
#ifdef BOARD_M5STICKC_PLUS2
    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.setTextSize(1);
    
    M5.Display.setTextColor(TFT_YELLOW);
    M5.Display.setCursor(5, 10);
    M5.Display.println("EAGLE-FIRMWARE");
    
    M5.Display.setTextColor(TFT_GREEN);
    M5.Display.setCursor(5, 30);
    M5.Display.println("Version: 1.0");
    
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.setCursor(5, 50);
    M5.Display.println("ESP32 Pentesting");
    M5.Display.setCursor(5, 65);
    M5.Display.println("Multi-Tool");
    
    M5.Display.setTextColor(TFT_CYAN);
    M5.Display.setCursor(5, 90);
    M5.Display.println("Features:");
    M5.Display.setTextSize(1);
    M5.Display.setTextColor(TFT_LIGHTGREY);
    M5.Display.setCursor(5, 105);
    M5.Display.println("WiFi/BLE/IR/RF Attacks");
    
    M5.Display.setTextColor(TFT_RED);
    M5.Display.setCursor(5, 125);
    M5.Display.println("Press B to go back");
    
    while(true) {
        M5.update();
        if (M5.BtnPWR.wasPressed()) break;
        delay(10);
    }
    drawMenu();
#endif
}

// ========== HELPERS ==========

std::vector<MenuItem>& MenuModule::getCurrentMenu() {
    switch(currentState) {
        case MENU_WIFI: return wifiMenu;
        case MENU_BLE: return bleMenu;
        case MENU_RF: return rfMenu;
        case MENU_RFID: return rfidMenu;
        case MENU_IR: return irMenu;
        case MENU_BADUSB: return badusbMenu;
        case MENU_OTHER: return otherMenu;
        case MENU_SETTINGS: return settingsMenu;
        default: return mainMenu;
    }
}

const char* MenuModule::getCurrentMenuTitle() {
    switch(currentState) {
        case MENU_WIFI: return "WiFi";
        case MENU_BLE: return "BLE";
        case MENU_RF: return "RF/SubGhz";
        case MENU_RFID: return "RFID/NFC";
        case MENU_IR: return "Infrared";
        case MENU_BADUSB: return "BadUSB";
        case MENU_OTHER: return "Other";
        case MENU_SETTINGS: return "Settings";
        default: return "Main Menu";
    }
}

void MenuModule::showMessage(const char* msg, uint16_t color, int duration) {
#ifdef BOARD_M5STICKC_PLUS2
    // Desenhar mensagem no topo
    M5.Display.fillRect(0, 20, 240, 30, TFT_BLACK);
    M5.Display.setTextSize(1);
    M5.Display.setTextColor(color);
    M5.Display.setCursor(10, 25);
    M5.Display.println(msg);
    
    Serial.printf("[Menu] %s\n", msg);
    
    if (duration > 0) {
        delay(duration);
        drawMenu();
    }
#endif
}
