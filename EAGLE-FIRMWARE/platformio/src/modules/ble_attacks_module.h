#ifndef BLE_ATTACKS_MODULE_H
#define BLE_ATTACKS_MODULE_H

#include "../core/module_manager.h"
#include <NimBLEDevice.h>
#include <NimBLEBeacon.h>
#include <NimBLEUtils.h>
#include <NimBLEServer.h>
#include <vector>

enum BLESpamType {
    SPAM_ALL,
    SPAM_IOS,
    SPAM_ANDROID,
    SPAM_SAMSUNG,
    SPAM_WINDOWS
};

class BLEAttacksModule : public IModule {
public:
    BLEAttacksModule() {}
    
    const char* getName() const override { return "BLE Attacks"; }
    void setup() override;
    void loop() override;
    
    // Attack methods
    void startBLESpam(BLESpamType type);
    void startAppleJuiceAttack();
    void startSwiftPairAttack();  // Windows spam
    void startFastPairAttack();   // Android spam
    void startSamsungSpam();
    void stopSpam();
    
    // Scan methods
    void scanBLEDevices(uint32_t duration = 5);
    
    // Status
    bool isSpamActive() const { return spamActive; }
    
private:
    bool spamActive = false;
    NimBLEAdvertising* pAdvertising = nullptr;
    
    // Apple devices data (AirPods, Beats, etc.)
    static const uint8_t appleDevices[];
    static const int appleDevicesCount;
    
    // Android Fast Pair device IDs
    static const uint32_t androidDevices[];
    static const int androidDevicesCount;
    
    // Samsung specific data
    static const uint8_t samsungModels[];
    static const int samsungModelsCount;
    
    // Windows Swift Pair data
    static const uint8_t windowsBeacons[];
    
    // Helper methods
    void sendAppleBeacon(uint8_t deviceType);
    void sendAndroidBeacon(uint32_t deviceID);
    void sendSamsungBeacon(uint8_t model);
    void sendWindowsBeacon();
    void setRandomMAC();
};

#endif // BLE_ATTACKS_MODULE_H
