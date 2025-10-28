#pragma once

#include <Arduino.h>
#ifdef BOARD_M5STICKC_PLUS2
#include <M5Unified.h>
#endif
#include "core/module_manager.h"
#include <vector>

// Forward declarations para os módulos de ataque
class WiFiAttacksModule;
class BLEAttacksModule;
class IRModule;
class EvilPortalModule;

enum MenuState {
    MENU_MAIN,
    MENU_WIFI,
    MENU_BLE,
    MENU_RF,
    MENU_RFID,
    MENU_IR,
    MENU_BADUSB,
    MENU_OTHER,
    MENU_SETTINGS
};

struct MenuItem {
    String name;
    uint16_t color;
    MenuState nextState;
    std::function<void()> action;
};

class MenuModule : public IModule {
public:
    MenuModule() : currentState(MENU_MAIN), selectedIndex(0), rotation(1) {}
    
    const char* getName() const override { return "menu"; }
    void setup() override;
    void loop() override;
    
    // Setters para módulos de ataque
    void setWiFiAttacks(WiFiAttacksModule* wifi) { wifiAttacks = wifi; }
    void setBLEAttacks(BLEAttacksModule* ble) { bleAttacks = ble; }
    void setIRModule(IRModule* ir) { irModule = ir; }
    void setEvilPortal(EvilPortalModule* portal) { evilPortal = portal; }
    
private:
    MenuState currentState;
    int selectedIndex;
    uint8_t rotation;
    unsigned long lastButtonPress;
    const unsigned long debounceDelay = 200;
    
    // Referências aos módulos
    WiFiAttacksModule* wifiAttacks = nullptr;
    BLEAttacksModule* bleAttacks = nullptr;
    IRModule* irModule = nullptr;
    EvilPortalModule* evilPortal = nullptr;
    
    // Menus
    std::vector<MenuItem> mainMenu;
    std::vector<MenuItem> wifiMenu;
    std::vector<MenuItem> bleMenu;
    std::vector<MenuItem> rfMenu;
    std::vector<MenuItem> rfidMenu;
    std::vector<MenuItem> irMenu;
    std::vector<MenuItem> badusbMenu;
    std::vector<MenuItem> otherMenu;
    std::vector<MenuItem> settingsMenu;
    
    // Métodos de inicialização
    void initMainMenu();
    void initWiFiMenu();
    void initBLEMenu();
    void initRFMenu();
    void initRFIDMenu();
    void initIRMenu();
    void initBadUSBMenu();
    void initOtherMenu();
    void initSettingsMenu();
    
    // Métodos de navegação
    void handleButtons();
    void navigateUp();
    void navigateDown();
    void selectItem();
    void goBack();
    void rotateScreen();
    
    // Métodos de desenho
    void drawMenu();
    void drawHeader(const char* title);
    void drawMenuItem(int index, const MenuItem& item, int y);
    void drawStatusBar();
    
    // Métodos de ação WiFi
    void actionWiFiScan();
    void actionWiFiBeaconSpam();
    void actionWiFiDeauthFlood();
    void actionWiFiEvilPortal();
    void actionWiFiSniffer();
    
    // Métodos de ação BLE
    void actionBLEScan();
    void actionBLESpamAll();
    void actionBLESpamIOS();
    void actionBLESpamAndroid();
    void actionBLESpamSamsung();
    void actionBLESpamWindows();
    
    // Métodos de ação IR
    void actionIRTVBGone();
    void actionIRLearn();
    void actionIRReplay();
    
    // Métodos de ação outros
    void actionStopAll();
    void actionSystemInfo();
    void actionAbout();
    
    // Helpers
    std::vector<MenuItem>& getCurrentMenu();
    const char* getCurrentMenuTitle();
    void showMessage(const char* msg, uint16_t color = TFT_WHITE, int duration = 2000);
};
