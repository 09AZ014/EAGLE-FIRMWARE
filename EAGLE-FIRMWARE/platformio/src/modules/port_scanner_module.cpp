#include "port_scanner_module.h"
#include "display_module.h"
#include <WiFiClient.h>

PortScannerModule::PortScannerModule() {
    targetIP = "";
    startPort = 1;
    endPort = 1000;
    timeout = 5000;
    isRunning = false;
}

PortScannerModule::~PortScannerModule() {
    stopScan();
}

void PortScannerModule::setup() {
    Serial.println("Port Scanner Module initialized");
    targetIP = "192.168.1.1"; // Default target
}

void PortScannerModule::loop() {
    if (isRunning) {
        // Scanning logic is handled in startScan()
        // This loop method is called periodically
    }
}

void PortScannerModule::setTarget(String ip) {
    targetIP = ip;
    Serial.println("Port Scanner target set to: " + targetIP);
}

void PortScannerModule::setPortRange(uint16_t start, uint16_t end) {
    startPort = start;
    endPort = end;
    Serial.printf("Port range set to: %d-%d\n", start, end);
}

void PortScannerModule::setTimeout(uint16_t timeoutMs) {
    timeout = timeoutMs;
    Serial.printf("Timeout set to: %d ms\n", timeout);
}

bool PortScannerModule::startScan() {
    if (targetIP == "" || isRunning) {
        return false;
    }
    
    isRunning = true;
    openPorts.clear();
    closedPorts.clear();
    filteredPorts.clear();
    results.clear();
    
    Serial.println("Starting port scan on " + targetIP);
    Serial.printf("Port range: %d-%d\n", startPort, endPort);
    
    int totalPorts = endPort - startPort + 1;
    int scanned = 0;
    
    for (int port = startPort; port <= endPort && isRunning; port++) {
        scanPort(port);
        scanned++;
        
        if (onProgress) {
            onProgress((scanned * 100) / totalPorts);
        }
        
        // Update display every 10 ports
        if (scanned % 10 == 0) {
            updateDisplay();
        }
        
        delay(10); // Small delay to prevent overwhelming the target
    }
    
    isRunning = false;
    
    if (onScanComplete) {
        onScanComplete();
    }
    
    Serial.println("Port scan completed");
    printResults();
    
    return true;
}

bool PortScannerModule::startQuickScan() {
    if (targetIP == "" || isRunning) {
        return false;
    }
    
    isRunning = true;
    openPorts.clear();
    closedPorts.clear();
    filteredPorts.clear();
    results.clear();
    
    Serial.println("Starting quick scan on " + targetIP);
    Serial.printf("Scanning %d common ports\n", commonPorts.size());
    
    int scanned = 0;
    int totalPorts = commonPorts.size();
    
    for (int port : commonPorts) {
        if (!isRunning) break;
        
        scanPort(port);
        scanned++;
        
        if (onProgress) {
            onProgress((scanned * 100) / totalPorts);
        }
        
        updateDisplay();
        delay(50); // Slightly longer delay for quick scan
    }
    
    isRunning = false;
    
    if (onScanComplete) {
        onScanComplete();
    }
    
    Serial.println("Quick scan completed");
    printResults();
    
    return true;
}

void PortScannerModule::stopScan() {
    isRunning = false;
    Serial.println("Port scan stopped");
}

void PortScannerModule::scanPort(int port) {
    WiFiClient client;
    client.setTimeout(timeout);
    
    PortResult result;
    result.port = port;
    result.service = "";
    result.version = "";
    result.responseTime = 0;
    result.isVulnerable = false;
    
    uint32_t startTime = millis();
    
    // Real port scanning with connection attempt
    if (client.connect(targetIP.c_str(), port)) {
        result.status = "open";
        result.responseTime = millis() - startTime;
        openPorts.push_back(port);
        
        // Try to identify service and get banner
        result.service = identifyService(port);
        result.version = getServiceBanner(client, port);
        
        // Check if service is vulnerable
        result.isVulnerable = checkVulnerability(port, result.service);
        
        client.stop();
        
        Serial.printf("Port %d: OPEN - %s (%s)\n", port, result.service.c_str(), result.version.c_str());
    } else {
        result.status = "closed";
        result.responseTime = millis() - startTime;
        closedPorts.push_back(port);
        
        Serial.printf("Port %d: CLOSED\n", port);
    }
    
    results.push_back(result);
    
    if (onPortScanned) {
        onPortScanned(port, result.status);
    }
}

String PortScannerModule::identifyService(int port) {
    // Common port to service mapping
    switch (port) {
        case 21: return "FTP";
        case 22: return "SSH";
        case 23: return "Telnet";
        case 25: return "SMTP";
        case 53: return "DNS";
        case 80: return "HTTP";
        case 110: return "POP3";
        case 135: return "RPC";
        case 139: return "NetBIOS";
        case 143: return "IMAP";
        case 443: return "HTTPS";
        case 993: return "IMAPS";
        case 995: return "POP3S";
        case 1723: return "PPTP";
        case 3389: return "RDP";
        case 5900: return "VNC";
        case 8080: return "HTTP-Alt";
        default: return "Unknown";
    }
}

String PortScannerModule::getServiceVersion(int port) {
    // This would typically involve banner grabbing
    // For now, return a placeholder
    return "Version detection not implemented";
}

String PortScannerModule::getServiceBanner(WiFiClient& client, int port) {
    String banner = "";
    String service = identifyService(port);
    
    // Wait a bit for the service to send banner
    delay(100);
    
    if (client.available()) {
        // Read banner based on service type
        if (service == "HTTP" || service == "HTTPS") {
            // Read HTTP headers
            while (client.available() && banner.length() < 500) {
                String line = client.readStringUntil('\n');
                if (line.startsWith("Server:") || line.startsWith("X-Powered-By:")) {
                    banner = line;
                    break;
                }
            }
        } else if (service == "SSH") {
            // Read SSH banner
            banner = client.readStringUntil('\n');
            banner.trim();
        } else if (service == "FTP") {
            // Read FTP banner
            banner = client.readStringUntil('\n');
            banner.trim();
        } else if (service == "SMTP") {
            // Read SMTP banner
            banner = client.readStringUntil('\n');
            banner.trim();
        } else if (service == "Telnet") {
            // Read Telnet banner
            banner = client.readStringUntil('\n');
            banner.trim();
        } else {
            // Generic banner reading
            banner = client.readStringUntil('\n');
            banner.trim();
        }
    }
    
    // Clean up banner
    banner.replace("\r", "");
    banner.replace("\n", "");
    if (banner.length() > 100) {
        banner = banner.substring(0, 100) + "...";
    }
    
    return banner;
}

bool PortScannerModule::isCommonPort(int port) {
    for (int commonPort : commonPorts) {
        if (port == commonPort) {
            return true;
        }
    }
    return false;
}

bool PortScannerModule::checkVulnerability(int port, String service) {
    // Basic vulnerability checks
    if (service == "Telnet" || service == "FTP") {
        return true; // These are generally considered insecure
    }
    if (port == 23 || port == 21) {
        return true; // Telnet and FTP ports
    }
    return false;
}

int PortScannerModule::getTotalPortsScanned() {
    return results.size();
}

void PortScannerModule::updateDisplay() {
    // This would integrate with the display module
    // For now, just print to serial
    Serial.printf("Progress: %d/%d ports scanned, %d open\n", 
                  getTotalPortsScanned(), 
                  endPort - startPort + 1, 
                  getOpenPortsCount());
}

void PortScannerModule::drawStatus() {
    // Integration with display module would go here
    Serial.println("=== Port Scanner Status ===");
    Serial.println("Target: " + targetIP);
    Serial.printf("Range: %d-%d\n", startPort, endPort);
    Serial.printf("Status: %s\n", isRunning ? "Running" : "Stopped");
    Serial.printf("Progress: %d ports scanned\n", getTotalPortsScanned());
}

void PortScannerModule::drawResults() {
    Serial.println("=== Port Scan Results ===");
    Serial.printf("Total Ports Scanned: %d\n", getTotalPortsScanned());
    Serial.printf("Open Ports: %d\n", getOpenPortsCount());
    Serial.printf("Closed Ports: %d\n", getClosedPortsCount());
    Serial.printf("Filtered Ports: %d\n", getFilteredPortsCount());
    
    Serial.println("\nOpen Ports:");
    for (int port : openPorts) {
        String service = identifyService(port);
        Serial.printf("Port %d - %s\n", port, service.c_str());
    }
}

void PortScannerModule::printResults() {
    drawResults();
}
