#ifndef WIFI_ATTACKS_MODULE_H
#define WIFI_ATTACKS_MODULE_H

#include "../core/module_manager.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <vector>

// Override raw frame sanity check (required for deauth attacks)
extern "C" int ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3);

class WiFiAttacksModule : public IModule {
public:
    WiFiAttacksModule() {}
    
    const char* getName() const override { return "WiFi Attacks"; }
    void setup() override;
    void loop() override;
    
    // Attack methods
    void startBeaconSpam(const std::vector<String>& ssids);
    void startDeauthAttack(const uint8_t* targetMAC, uint8_t channel);
    void startDeauthFlood();
    void startTargetedDeauth(const String& ssid, const uint8_t* bssid, uint8_t channel);
    
    // Scan methods
    std::vector<wifi_ap_record_t> scanNetworks();
    
    // Status
    bool isAttackActive() const { return attackActive; }
    void stopAttack();
    
private:
    bool attackActive = false;
    
    // Deauth frame structures
    static const uint8_t deauthFrameDefault[26];
    uint8_t deauthFrame[26];
    
    // Helper methods
    void sendRawFrame(const uint8_t* frameBuffer, int size);
    void prepareDeauthFrame(const wifi_ap_record_t* apRecord, uint8_t channel, 
                            const uint8_t target[6] = nullptr);
    void sendBeaconFrame(const String& ssid, uint8_t channel);
};

#endif // WIFI_ATTACKS_MODULE_H
