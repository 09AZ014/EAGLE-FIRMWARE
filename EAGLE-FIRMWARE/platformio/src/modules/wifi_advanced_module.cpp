#include "wifi_advanced_module.h"
#include "display_module.h"
#include <esp_wifi.h>
#include <esp_err.h>

WiFiAdvancedModule::WiFiAdvancedModule() {
    isAPMode = false;
    isConnected = false;
    apSSID = "EAGLE-AP";
    apPassword = "";
    apIP = IPAddress(192, 168, 4, 1);
    evilPortalActive = false;
    wardrivingActive = false;
    rawSnifferActive = false;
    telnetConnected = false;
    sshConnected = false;
    tcpConnected = false;
    wireguardActive = false;
}

WiFiAdvancedModule::~WiFiAdvancedModule() {
    stopAllAttacks();
    stopEvilPortal();
    stopWardriving();
    stopRawSniffer();
    disconnectWiFi();
    stopAccessPoint();
}

void WiFiAdvancedModule::setup() {
    Serial.println("WiFi Advanced Module initialized");
    
    // Initialize WiFi in STA mode
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    
    // Set up captive portal HTML
    captivePortalHTML = R"(
<!DOCTYPE html>
<html>
<head>
    <title>WiFi Login</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }
        .container { max-width: 400px; margin: 0 auto; padding: 20px; }
        input { width: 100%; padding: 10px; margin: 10px 0; }
        button { background: #007bff; color: white; padding: 10px 20px; border: none; cursor: pointer; }
    </style>
</head>
<body>
    <div class="container">
        <h2>WiFi Login Required</h2>
        <form method="post" action="/login">
            <input type="text" name="username" placeholder="Username" required>
            <input type="password" name="password" placeholder="Password" required>
            <button type="submit">Login</button>
        </form>
    </div>
</body>
</html>
    )";
}

void WiFiAdvancedModule::loop() {
    // Handle wardriving
    if (wardrivingActive) {
        scanNetworks();
    }
    
    // Handle raw sniffer
    if (rawSnifferActive) {
        // Raw packet processing would go here
        // This requires low-level WiFi access
    }
    
    // Handle TCP server
    if (tcpServer.hasClient()) {
        WiFiClient client = tcpServer.available();
        if (client) {
            String request = client.readStringUntil('\r');
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
            client.println("<h1>EAGLE TCP Server</h1>");
            client.println("<p>Connected to EAGLE Firmware</p>");
            client.stop();
        }
    }
}

bool WiFiAdvancedModule::connectToWiFi(String ssid, String password) {
    Serial.println("Connecting to WiFi: " + ssid);
    
    WiFi.begin(ssid.c_str(), password.c_str());
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        isConnected = true;
        Serial.println("\nWiFi connected!");
        Serial.println("IP address: " + WiFi.localIP().toString());
        return true;
    } else {
        isConnected = false;
        Serial.println("\nWiFi connection failed!");
        return false;
    }
}

bool WiFiAdvancedModule::disconnectWiFi() {
    WiFi.disconnect();
    isConnected = false;
    Serial.println("WiFi disconnected");
    return true;
}

bool WiFiAdvancedModule::createAccessPoint(String ssid, String password) {
    Serial.println("Creating Access Point: " + ssid);
    
    apSSID = ssid;
    apPassword = password;
    
    WiFi.mode(WIFI_AP);
    
    if (password.length() > 0) {
        WiFi.softAP(ssid.c_str(), password.c_str());
    } else {
        WiFi.softAP(ssid.c_str());
    }
    
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    
    isAPMode = true;
    Serial.println("Access Point created!");
    Serial.println("SSID: " + apSSID);
    Serial.println("IP: " + apIP.toString());
    
    return true;
}

bool WiFiAdvancedModule::stopAccessPoint() {
    WiFi.softAPdisconnect(true);
    isAPMode = false;
    Serial.println("Access Point stopped");
    return true;
}

void WiFiAdvancedModule::startNetworkScan() {
    Serial.println("Starting network scan...");
    WiFi.scanNetworks(true); // Async scan
}

void WiFiAdvancedModule::stopNetworkScan() {
    WiFi.scanDelete();
    Serial.println("Network scan stopped");
}

bool WiFiAdvancedModule::isScanning() {
    return WiFi.scanComplete() == WIFI_SCAN_RUNNING;
}

void WiFiAdvancedModule::scanNetworks() {
    int n = WiFi.scanComplete();
    if (n == WIFI_SCAN_FAILED) {
        Serial.println("Scan failed");
        return;
    } else if (n == WIFI_SCAN_RUNNING) {
        return; // Still scanning
    }
    
    networks.clear();
    
    for (int i = 0; i < n; i++) {
        WiFiNetwork network;
        network.ssid = WiFi.SSID(i);
        network.bssid = WiFi.BSSIDstr(i);
        network.rssi = WiFi.RSSI(i);
        network.channel = WiFi.channel(i);
        network.authMode = WiFi.encryptionType(i);
        network.isHidden = network.ssid.length() == 0;
        network.timestamp = millis();
        
        networks.push_back(network);
        
        if (onNetworkFound) {
            onNetworkFound(network);
        }
        
        Serial.printf("Network %d: %s (%s) Ch:%d RSSI:%d\n", 
                     i, network.ssid.c_str(), network.bssid.c_str(), 
                     network.channel, network.rssi);
    }
    
    if (wardrivingActive) {
        wardrivingData.insert(wardrivingData.end(), networks.begin(), networks.end());
    }
}

bool WiFiAdvancedModule::startBeaconSpamAttack(String ssid) {
    Serial.println("Starting Beacon Spam attack: " + ssid);
    
    WiFiAttack attack;
    attack.targetSSID = ssid;
    attack.isRunning = true;
    attack.packetsSent = 0;
    attack.startTime = millis();
    
    activeAttacks.push_back(attack);
    
    if (onAttackStarted) {
        onAttackStarted(attack);
    }
    
    return true;
}

bool WiFiAdvancedModule::startDeauthAttack(String targetSSID, String targetBSSID, uint8_t channel) {
    Serial.println("Starting Deauth attack: " + targetSSID + " (" + targetBSSID + ")");
    
    WiFiAttack attack;
    attack.targetSSID = targetSSID;
    attack.targetBSSID = targetBSSID;
    attack.channel = channel;
    attack.isRunning = true;
    attack.packetsSent = 0;
    attack.startTime = millis();
    
    activeAttacks.push_back(attack);
    
    if (onAttackStarted) {
        onAttackStarted(attack);
    }
    
    return true;
}

bool WiFiAdvancedModule::stopAllAttacks() {
    for (auto& attack : activeAttacks) {
        attack.isRunning = false;
        if (onAttackStopped) {
            onAttackStopped(attack);
        }
    }
    activeAttacks.clear();
    Serial.println("All attacks stopped");
    return true;
}

bool WiFiAdvancedModule::startEvilPortal(String ssid, String password, String html) {
    Serial.println("Starting Evil Portal: " + ssid);
    
    evilPortalSSID = ssid;
    evilPortalPassword = password;
    if (html.length() > 0) {
        captivePortalHTML = html;
    }
    
    // Create AP with captive portal
    createAccessPoint(ssid, password);
    
    // Start web server for captive portal
    // This would require AsyncWebServer implementation
    
    evilPortalActive = true;
    Serial.println("Evil Portal started");
    return true;
}

bool WiFiAdvancedModule::stopEvilPortal() {
    stopAccessPoint();
    evilPortalActive = false;
    Serial.println("Evil Portal stopped");
    return true;
}

bool WiFiAdvancedModule::startWardriving() {
    Serial.println("Starting Wardriving");
    wardrivingActive = true;
    wardrivingData.clear();
    startNetworkScan();
    return true;
}

bool WiFiAdvancedModule::stopWardriving() {
    Serial.println("Stopping Wardriving");
    wardrivingActive = false;
    saveWardrivingData();
    return true;
}

void WiFiAdvancedModule::saveWardrivingData() {
    Serial.println("Saving wardriving data...");
    Serial.printf("Total networks found: %d\n", wardrivingData.size());
    
    // Save to LittleFS or SD card
    // Implementation would go here
}

bool WiFiAdvancedModule::startRawSniffer() {
    Serial.println("Starting Raw Sniffer");
    rawSnifferActive = true;
    capturedPackets.clear();
    return true;
}

bool WiFiAdvancedModule::stopRawSniffer() {
    Serial.println("Stopping Raw Sniffer");
    rawSnifferActive = false;
    return true;
}

bool WiFiAdvancedModule::connectTelnet(String host, uint16_t port) {
    Serial.println("Connecting to Telnet: " + host + ":" + String(port));
    
    if (telnetClient.connect(host.c_str(), port)) {
        telnetConnected = true;
        Serial.println("Telnet connected");
        return true;
    } else {
        telnetConnected = false;
        Serial.println("Telnet connection failed");
        return false;
    }
}

bool WiFiAdvancedModule::disconnectTelnet() {
    telnetClient.stop();
    telnetConnected = false;
    Serial.println("Telnet disconnected");
    return true;
}

void WiFiAdvancedModule::sendTelnetCommand(String command) {
    if (telnetConnected) {
        telnetClient.println(command);
    }
}

String WiFiAdvancedModule::readTelnetResponse() {
    if (telnetConnected && telnetClient.available()) {
        return telnetClient.readString();
    }
    return "";
}

bool WiFiAdvancedModule::connectSSH(String host, uint16_t port) {
    Serial.println("Connecting to SSH: " + host + ":" + String(port));
    
    // Simplified SSH connection (would need proper SSH library)
    if (sshClient.connect(host.c_str(), port)) {
        sshConnected = true;
        Serial.println("SSH connected");
        return true;
    } else {
        sshConnected = false;
        Serial.println("SSH connection failed");
        return false;
    }
}

bool WiFiAdvancedModule::disconnectSSH() {
    sshClient.stop();
    sshConnected = false;
    Serial.println("SSH disconnected");
    return true;
}

void WiFiAdvancedModule::sendSSHCommand(String command) {
    if (sshConnected) {
        sshClient.println(command);
    }
}

String WiFiAdvancedModule::readSSHResponse() {
    if (sshConnected && sshClient.available()) {
        return sshClient.readString();
    }
    return "";
}

bool WiFiAdvancedModule::connectTCP(String host, uint16_t port) {
    Serial.println("Connecting to TCP: " + host + ":" + String(port));
    
    if (tcpClient.connect(host.c_str(), port)) {
        tcpConnected = true;
        Serial.println("TCP connected");
        return true;
    } else {
        tcpConnected = false;
        Serial.println("TCP connection failed");
        return false;
    }
}

bool WiFiAdvancedModule::disconnectTCP() {
    tcpClient.stop();
    tcpConnected = false;
    Serial.println("TCP disconnected");
    return true;
}

void WiFiAdvancedModule::sendTCPData(String data) {
    if (tcpConnected) {
        tcpClient.print(data);
    }
}

String WiFiAdvancedModule::readTCPData() {
    if (tcpConnected && tcpClient.available()) {
        return tcpClient.readString();
    }
    return "";
}

bool WiFiAdvancedModule::startTCPServer(uint16_t port) {
    Serial.println("Starting TCP Server on port: " + String(port));
    tcpServer.begin(port);
    return true;
}

bool WiFiAdvancedModule::stopTCPServer() {
    tcpServer.stop();
    Serial.println("TCP Server stopped");
    return true;
}

bool WiFiAdvancedModule::isTCPServerActive() {
    return tcpServer.status() == WL_LISTEN;
}

WiFiClient WiFiAdvancedModule::acceptTCPClient() {
    return tcpServer.available();
}

bool WiFiAdvancedModule::startWireGuard(String config) {
    Serial.println("Starting WireGuard");
    wireguardConfig = config;
    wireguardActive = true;
    // WireGuard implementation would go here
    return true;
}

bool WiFiAdvancedModule::stopWireGuard() {
    Serial.println("Stopping WireGuard");
    wireguardActive = false;
    return true;
}

String WiFiAdvancedModule::getNetworkInfo() {
    String info = "WiFi Status: ";
    if (isConnected) {
        info += "Connected\n";
        info += "SSID: " + WiFi.SSID() + "\n";
        info += "IP: " + WiFi.localIP().toString() + "\n";
        info += "RSSI: " + String(WiFi.RSSI()) + " dBm\n";
        info += "Channel: " + String(WiFi.channel()) + "\n";
    } else if (isAPMode) {
        info += "AP Mode\n";
        info += "SSID: " + apSSID + "\n";
        info += "IP: " + apIP.toString() + "\n";
    } else {
        info += "Disconnected\n";
    }
    return info;
}

String WiFiAdvancedModule::getSignalStrength() {
    if (isConnected) {
        return String(WiFi.RSSI()) + " dBm";
    }
    return "N/A";
}

String WiFiAdvancedModule::getChannelInfo() {
    if (isConnected) {
        return "Channel " + String(WiFi.channel());
    }
    return "N/A";
}

String WiFiAdvancedModule::getMACAddress() {
    return WiFi.macAddress();
}

void WiFiAdvancedModule::drawStatus() {
    Serial.println("=== WiFi Advanced Status ===");
    Serial.println(getNetworkInfo());
    Serial.println("Active Attacks: " + String(activeAttacks.size()));
    Serial.println("Networks Found: " + String(networks.size()));
    Serial.println("Wardriving: " + String(wardrivingActive ? "Active" : "Inactive"));
    Serial.println("Raw Sniffer: " + String(rawSnifferActive ? "Active" : "Inactive"));
}

void WiFiAdvancedModule::drawNetworks() {
    Serial.println("=== Discovered Networks ===");
    for (int i = 0; i < networks.size(); i++) {
        WiFiNetwork& net = networks[i];
        Serial.printf("%d: %s (%s) Ch:%d RSSI:%d\n", 
                     i, net.ssid.c_str(), net.bssid.c_str(), 
                     net.channel, net.rssi);
    }
}

void WiFiAdvancedModule::drawAttacks() {
    Serial.println("=== Active Attacks ===");
    for (int i = 0; i < activeAttacks.size(); i++) {
        WiFiAttack& attack = activeAttacks[i];
        Serial.printf("%d: %s - Packets: %d\n", 
                     i, attack.targetSSID.c_str(), attack.packetsSent);
    }
}

void WiFiAdvancedModule::drawWardrivingData() {
    Serial.println("=== Wardriving Data ===");
    Serial.printf("Total Networks: %d\n", wardrivingData.size());
    for (int i = 0; i < min(10, (int)wardrivingData.size()); i++) {
        WiFiNetwork& net = wardrivingData[i];
        Serial.printf("%d: %s (%s) Ch:%d RSSI:%d\n", 
                     i, net.ssid.c_str(), net.bssid.c_str(), 
                     net.channel, net.rssi);
    }
}
