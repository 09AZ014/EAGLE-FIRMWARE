#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_err.h>
#include <vector>
#include <functional>
#include "core/module_manager.h"

struct WiFiNetwork {
    String ssid;
    String bssid;
    int32_t rssi;
    uint8_t channel;
    wifi_auth_mode_t authMode;
    bool isHidden;
    String country;
    uint32_t timestamp;
};

struct WiFiAttack {
    String targetSSID;
    String targetBSSID;
    uint8_t channel;
    int32_t rssi;
    bool isRunning;
    uint32_t packetsSent;
    uint32_t startTime;
};

class WiFiAdvancedModule : public IModule {
private:
    bool isAPMode;
    bool isConnected;
    String apSSID;
    String apPassword;
    IPAddress apIP;
    std::vector<WiFiNetwork> networks;
    std::vector<WiFiAttack> activeAttacks;
    
    // Evil Portal
    bool evilPortalActive;
    String evilPortalSSID;
    String evilPortalPassword;
    String captivePortalHTML;
    
    // Wardriving
    bool wardrivingActive;
    std::vector<WiFiNetwork> wardrivingData;
    
    // Raw Sniffer
    bool rawSnifferActive;
    std::vector<String> capturedPackets;
    
    // Telnet/SSH/TCP
    WiFiClient telnetClient;
    WiFiClient sshClient;
    WiFiClient tcpClient;
    WiFiServer tcpServer;
    bool telnetConnected;
    bool sshConnected;
    bool tcpConnected;
    
    // WireGuard
    bool wireguardActive;
    String wireguardConfig;
    
    void scanNetworks();
    void startBeaconSpam();
    void startDeauthAttack(String targetSSID, String targetBSSID);
    void startEvilPortal();
    void stopEvilPortal();
    void startWardriving();
    void stopWardriving();
    void startRawSniffer();
    void stopRawSniffer();
    void processRawPacket(uint8_t* data, uint16_t len);
    
    // Attack implementations
    void sendDeauthFrame(String targetBSSID, String sourceBSSID, uint8_t channel);
    void sendBeaconFrame(String ssid, uint8_t channel);
    void sendProbeRequest(String ssid);
    
    // Network analysis
    void analyzeNetwork(WiFiNetwork& network);
    String getAuthModeString(wifi_auth_mode_t authMode);
    String getEncryptionType(wifi_auth_mode_t authMode);
    
public:
    WiFiAdvancedModule();
    ~WiFiAdvancedModule();
    
    const char* getName() const override { return "wifi_advanced"; }
    void setup() override;
    void loop() override;
    
    // WiFi Connection
    bool connectToWiFi(String ssid, String password);
    bool disconnectWiFi();
    bool isWiFiConnected() { return isConnected; }
    String getWiFiSSID();
    IPAddress getWiFiIP();
    
    // Access Point
    bool createAccessPoint(String ssid, String password = "");
    bool stopAccessPoint();
    bool isAPActive() { return isAPMode; }
    String getAPSSID() { return apSSID; }
    IPAddress getAPIP() { return apIP; }
    
    // Network Scanning
    std::vector<WiFiNetwork> getNetworks() { return networks; }
    void startNetworkScan();
    void stopNetworkScan();
    bool isScanning();
    
    // WiFi Attacks
    bool startBeaconSpamAttack(String ssid);
    bool startDeauthAttack(String targetSSID, String targetBSSID, uint8_t channel);
    bool stopAllAttacks();
    std::vector<WiFiAttack> getActiveAttacks() { return activeAttacks; }
    
    // Evil Portal
    bool startEvilPortal(String ssid, String password, String html);
    bool isEvilPortalActive() { return evilPortalActive; }
    void setCaptivePortalHTML(String html);
    
    // Wardriving
    bool isWardrivingActive() { return wardrivingActive; }
    std::vector<WiFiNetwork> getWardrivingData() { return wardrivingData; }
    void saveWardrivingData();
    
    // Raw Sniffer
    bool isRawSnifferActive() { return rawSnifferActive; }
    std::vector<String> getCapturedPackets() { return capturedPackets; }
    void clearCapturedPackets();
    
    // Telnet Client
    bool connectTelnet(String host, uint16_t port);
    bool disconnectTelnet();
    bool isTelnetConnected() { return telnetConnected; }
    void sendTelnetCommand(String command);
    String readTelnetResponse();
    
    // SSH Client (simplified)
    bool connectSSH(String host, uint16_t port);
    bool disconnectSSH();
    bool isSSHConnected() { return sshConnected; }
    void sendSSHCommand(String command);
    String readSSHResponse();
    
    // TCP Client/Server
    bool connectTCP(String host, uint16_t port);
    bool disconnectTCP();
    bool isTCPConnected() { return tcpConnected; }
    void sendTCPData(String data);
    String readTCPData();
    
    bool startTCPServer(uint16_t port);
    bool stopTCPServer();
    bool isTCPServerActive();
    WiFiClient acceptTCPClient();
    
    // WireGuard
    bool startWireGuard(String config);
    bool stopWireGuard();
    bool isWireGuardActive() { return wireguardActive; }
    
    // Network Information
    String getNetworkInfo();
    String getSignalStrength();
    String getChannelInfo();
    String getMACAddress();
    
    // Callbacks
    std::function<void(WiFiNetwork network)> onNetworkFound;
    std::function<void(WiFiAttack attack)> onAttackStarted;
    std::function<void(WiFiAttack attack)> onAttackStopped;
    std::function<void(String packet)> onPacketCaptured;
    
    // Display integration
    void drawStatus();
    void drawNetworks();
    void drawAttacks();
    void drawWardrivingData();
};
