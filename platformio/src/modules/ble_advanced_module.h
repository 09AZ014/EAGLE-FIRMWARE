#pragma once

#include <Arduino.h>
#include <NimBLEDevice.h>
#include <NimBLEClient.h>
#include <NimBLEServer.h>
#include <NimBLEAdvertisedDevice.h>
#include <vector>
#include <functional>
#include "core/module_manager.h"

struct BLEDevice {
    String address;
    String name;
    int32_t rssi;
    uint8_t* manufacturerData;
    size_t manufacturerDataLength;
    std::vector<String> serviceUUIDs;
    std::vector<String> characteristicUUIDs;
    bool isConnectable;
    bool isConnected;
    uint32_t timestamp;
};

struct BLEAttack {
    String targetAddress;
    String targetName;
    bool isRunning;
    uint32_t packetsSent;
    uint32_t startTime;
    String attackType; // "spam", "badble", "keyboard"
};

class BLEAdvancedModule : public IModule {
private:
    NimBLEScan* pBLEScan;
    NimBLEClient* pClient;
    NimBLEServer* pServer;
    std::vector<BLEDevice> discoveredDevices;
    std::vector<BLEAttack> activeAttacks;
    
    bool isScanning;
    bool isAdvertising;
    bool isConnected;
    bool keyboardMode;
    
    // Bad BLE / Ducky functionality
    String duckyScript;
    bool duckyRunning;
    uint32_t duckyPosition;
    
    // BLE Keyboard (for Cardputer and T-Deck)
    bool keyboardConnected;
    String keyboardBuffer;
    
    // Spam functionality
    bool spamActive;
    String spamMessage;
    uint32_t spamInterval;
    
    void startBLEScan();
    void stopBLEScan();
    void startBLEAdvertising();
    void stopBLEAdvertising();
    void processDiscoveredDevice(NimBLEAdvertisedDevice* device);
    void executeDuckyScript();
    void sendKeyboardInput(String input);
    void startSpamAttack();
    void stopSpamAttack();
    
    // BLE Callbacks
    class MyAdvertisedDeviceCallbacks : public NimBLEAdvertisedDeviceCallbacks {
    public:
        BLEAdvancedModule* module;
        MyAdvertisedDeviceCallbacks(BLEAdvancedModule* mod) : module(mod) {}
        void onResult(NimBLEAdvertisedDevice* device) override;
    };
    
    class MyServerCallbacks : public NimBLEServerCallbacks {
    public:
        BLEAdvancedModule* module;
        MyServerCallbacks(BLEAdvancedModule* mod) : module(mod) {}
        void onConnect(NimBLEServer* pServer) override;
        void onDisconnect(NimBLEServer* pServer) override;
    };
    
    class MyCharacteristicCallbacks : public NimBLECharacteristicCallbacks {
    public:
        BLEAdvancedModule* module;
        MyCharacteristicCallbacks(BLEAdvancedModule* mod) : module(mod) {}
        void onWrite(NimBLECharacteristic* pCharacteristic) override;
        void onRead(NimBLECharacteristic* pCharacteristic) override;
    };
    
    MyAdvertisedDeviceCallbacks* pAdvertisedDeviceCallbacks;
    MyServerCallbacks* pServerCallbacks;
    MyCharacteristicCallbacks* pCharacteristicCallbacks;
    
public:
    BLEAdvancedModule();
    ~BLEAdvancedModule();
    
    const char* getName() const override { return "ble_advanced"; }
    void setup() override;
    void loop() override;
    
    // BLE Scanning
    bool startScan(uint32_t duration = 0);
    bool stopScan();
    bool isScanningActive() { return isScanning; }
    std::vector<BLEDevice> getDiscoveredDevices() { return discoveredDevices; }
    void clearDiscoveredDevices();
    
    // BLE Advertising
    bool startAdvertising(String name, String serviceUUID = "");
    bool stopAdvertising();
    bool isAdvertisingActive() { return isAdvertising; }
    
    // BLE Connection
    bool connectToDevice(String address);
    bool disconnectFromDevice();
    bool isDeviceConnected() { return isConnected; }
    
    // Bad BLE / Ducky
    bool loadDuckyScript(String script);
    bool startDuckyScript();
    bool stopDuckyScript();
    bool isDuckyRunning() { return duckyRunning; }
    String getDuckyScript() { return duckyScript; }
    void setDuckyScript(String script);
    
    // BLE Keyboard (Cardputer/T-Deck)
    bool startKeyboardMode();
    bool stopKeyboardMode();
    bool isKeyboardModeActive() { return keyboardMode; }
    void sendKeyboardString(String text);
    void sendKeyboardKey(uint8_t key, uint8_t modifiers = 0);
    void sendKeyboardSpecialKey(uint8_t key);
    
    // BLE Spam
    bool startSpamAll(String message, uint32_t interval = 1000);
    bool stopSpamAll();
    bool isSpamActive() { return spamActive; }
    void setSpamMessage(String message);
    void setSpamInterval(uint32_t interval);
    
    // Device Management
    BLEDevice* getDeviceByAddress(String address);
    BLEDevice* getDeviceByName(String name);
    bool connectToDevice(BLEDevice* device);
    
    // Attack Management
    bool startAttack(String targetAddress, String attackType);
    bool stopAttack(String targetAddress);
    bool stopAllAttacks();
    std::vector<BLEAttack> getActiveAttacks() { return activeAttacks; }
    
    // Service Discovery
    std::vector<String> getDeviceServices(String address);
    std::vector<String> getDeviceCharacteristics(String address, String serviceUUID);
    bool readCharacteristic(String address, String serviceUUID, String characteristicUUID);
    bool writeCharacteristic(String address, String serviceUUID, String characteristicUUID, String value);
    
    // Information
    String getBLEDeviceInfo();
    String getScanResults();
    String getAttackStatus();
    
    // Callbacks
    std::function<void(BLEDevice device)> onDeviceDiscovered;
    std::function<void(BLEDevice device)> onDeviceConnected;
    std::function<void(BLEDevice device)> onDeviceDisconnected;
    std::function<void(BLEAttack attack)> onAttackStarted;
    std::function<void(BLEAttack attack)> onAttackStopped;
    std::function<void(String data)> onDataReceived;
    
    // Display integration
    void drawStatus();
    void drawDevices();
    void drawAttacks();
    void drawKeyboardStatus();
};
