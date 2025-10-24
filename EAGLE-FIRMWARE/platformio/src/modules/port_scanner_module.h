#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <vector>
#include <functional>
#include "core/module_manager.h"

struct PortResult {
    int port;
    String status; // "open", "closed", "filtered", "error"
    String service;
    String version;
    uint32_t responseTime;
    bool isVulnerable;
};

class PortScannerModule : public IModule {
private:
    String targetIP;
    uint16_t startPort;
    uint16_t endPort;
    uint16_t timeout;
    bool isRunning;
    std::vector<PortResult> results;
    std::vector<int> openPorts;
    std::vector<int> closedPorts;
    std::vector<int> filteredPorts;
    
    // Common ports for quick scan
    std::vector<int> commonPorts = {21, 22, 23, 25, 53, 80, 110, 135, 139, 143, 443, 993, 995, 1723, 3389, 5900, 8080};
    
    void scanPort(int port);
    String identifyService(int port);
    String getServiceVersion(int port);
    String getServiceBanner(WiFiClient& client, int port);
    bool isCommonPort(int port);
    void updateDisplay();
    
public:
    PortScannerModule();
    ~PortScannerModule();
    
    const char* getName() const override { return "port_scanner"; }
    void setup() override;
    void loop() override;
    
    // Configuration
    void setTarget(String ip);
    void setPortRange(uint16_t start, uint16_t end);
    void setTimeout(uint16_t timeoutMs);
    
    // Scanning operations
    bool startScan();
    bool startQuickScan();
    void stopScan();
    bool isScanning() { return isRunning; }
    
    // Results
    std::vector<int> getOpenPorts() { return openPorts; }
    std::vector<int> getClosedPorts() { return closedPorts; }
    std::vector<int> getFilteredPorts() { return filteredPorts; }
    std::vector<PortResult> getDetailedResults() { return results; }
    
    // Statistics
    int getTotalPortsScanned();
    int getOpenPortsCount() { return openPorts.size(); }
    int getClosedPortsCount() { return closedPorts.size(); }
    int getFilteredPortsCount() { return filteredPorts.size(); }
    
    // Callbacks
    std::function<void(int port, String status)> onPortScanned;
    std::function<void(int progress)> onProgress;
    std::function<void()> onScanComplete;
    
    // Display integration
    void drawStatus();
    void drawResults();
    void printResults();
    
    // Vulnerability checking
    bool checkVulnerability(int port, String service);
};
