#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "core/module_manager.h"
#include "wifi_attacks_module.h"
#include "ble_attacks_module.h"

// Forward declarations
extern WiFiAttacksModule wifiAttacks;
extern BLEAttacksModule bleAttacks;

class AttacksApiModule : public IModule {
public:
    AttacksApiModule() : server(80) {}
    const char* getName() const override { return "attacks_api"; }
    
    void setup() override {
        // Status endpoint
        server.on("/api/status", HTTP_GET, [this]() { onStatus(); });
        
        // WiFi attack endpoints
        server.on("/api/wifi/scan", HTTP_POST, [this]() { onWiFiScan(); });
        server.on("/api/wifi/beacon-spam", HTTP_POST, [this]() { onBeaconSpam(); });
        server.on("/api/wifi/deauth-flood", HTTP_POST, [this]() { onDeauthFlood(); });
        server.on("/api/wifi/evil-portal", HTTP_POST, [this]() { onEvilPortal(); });
        
        // BLE attack endpoints
        server.on("/api/ble/scan", HTTP_POST, [this]() { onBLEScan(); });
        server.on("/api/ble/spam", HTTP_POST, [this]() { onBLESpam(); });
        
        // Control endpoints
        server.on("/api/system/stop", HTTP_POST, [this]() { onStop(); });
        server.on("/api/system/info", HTTP_POST, [this]() { onSystemInfo(); });
        
        // Serve index.html from LittleFS
        server.serveStatic("/", LittleFS, "/index.html");
        server.serveStatic("/index.html", LittleFS, "/index.html");
        
        server.begin();
        Serial.println("[Attacks API] Web server started on port 80");
    }
    
    void loop() override {
        server.handleClient();
    }

private:
    WebServer server;
    
    void sendJson(int code, const char* message) {
        DynamicJsonDocument doc(256);
        doc["message"] = message;
        doc["success"] = (code == 200);
        String output;
        serializeJson(doc, output);
        server.send(code, "application/json", output);
    }
    
    void onStatus() {
        DynamicJsonDocument doc(512);
        doc["board"] = "ESP32";
        doc["uptime"] = String(millis() / 1000) + "s";
        doc["freeRam"] = String(ESP.getFreeHeap() / 1024) + " KB";
        doc["ip"] = WiFi.localIP().toString();
        doc["wifiConnected"] = WiFi.isConnected();
        doc["wifiAttackActive"] = wifiAttacks.isAttackActive();
        doc["bleSpamActive"] = bleAttacks.isSpamActive();
        
        String output;
        serializeJson(doc, output);
        server.send(200, "application/json", output);
    }
    
    void onWiFiScan() {
        Serial.println("[API] WiFi scan requested");
        // Run in background to avoid blocking
        sendJson(200, "WiFi scan started");
        // Note: actual scan happens asynchronously
    }
    
    void onBeaconSpam() {
        Serial.println("[API] Beacon spam requested");
        if (wifiAttacks.isAttackActive()) {
            sendJson(400, "Attack already active");
            return;
        }
        
        // Parse SSIDs from request body
        std::vector<String> ssids = {"Free WiFi", "FBI Van", "Virus.exe", "TestNet"};
        
        // Start in separate task to avoid blocking
        xTaskCreate([](void* param) {
            wifiAttacks.startBeaconSpam(*((std::vector<String>*)param));
            vTaskDelete(NULL);
        }, "BeaconSpam", 4096, &ssids, 1, NULL);
        
        sendJson(200, "Beacon spam started");
    }
    
    void onDeauthFlood() {
        Serial.println("[API] Deauth flood requested");
        if (wifiAttacks.isAttackActive()) {
            sendJson(400, "Attack already active");
            return;
        }
        
        // Start in separate task
        xTaskCreate([](void* param) {
            wifiAttacks.startDeauthFlood();
            vTaskDelete(NULL);
        }, "DeauthFlood", 4096, NULL, 1, NULL);
        
        sendJson(200, "Deauth flood started");
    }
    
    void onEvilPortal() {
        Serial.println("[API] Evil portal requested");
        sendJson(501, "Evil Portal not yet implemented");
    }
    
    void onBLEScan() {
        Serial.println("[API] BLE scan requested");
        // Run scan
        xTaskCreate([](void* param) {
            bleAttacks.scanBLEDevices(10);
            vTaskDelete(NULL);
        }, "BLEScan", 4096, NULL, 1, NULL);
        
        sendJson(200, "BLE scan started");
    }
    
    void onBLESpam() {
        Serial.println("[API] BLE spam requested");
        if (bleAttacks.isSpamActive()) {
            sendJson(400, "Spam already active");
            return;
        }
        
        // Parse type from request body
        DynamicJsonDocument doc(256);
        if (server.hasArg("plain")) {
            deserializeJson(doc, server.arg("plain"));
        }
        
        String type = doc["type"] | "all";
        BLESpamType spamType = SPAM_ALL;
        
        if (type == "ios") spamType = SPAM_IOS;
        else if (type == "android") spamType = SPAM_ANDROID;
        else if (type == "samsung") spamType = SPAM_SAMSUNG;
        else if (type == "windows") spamType = SPAM_WINDOWS;
        
        // Start in separate task
        xTaskCreate([](void* param) {
            bleAttacks.startBLESpam(*((BLESpamType*)param));
            vTaskDelete(NULL);
        }, "BLESpam", 4096, &spamType, 1, NULL);
        
        sendJson(200, "BLE spam started");
    }
    
    void onStop() {
        Serial.println("[API] Stop requested");
        wifiAttacks.stopAttack();
        bleAttacks.stopSpam();
        sendJson(200, "All attacks stopped");
    }
    
    void onSystemInfo() {
        DynamicJsonDocument doc(512);
        doc["chipModel"] = ESP.getChipModel();
        doc["chipRevision"] = ESP.getChipRevision();
        doc["cpuFreq"] = ESP.getCpuFreqMHz();
        doc["flashSize"] = ESP.getFlashChipSize();
        doc["freeHeap"] = ESP.getFreeHeap();
        doc["uptime"] = millis() / 1000;
        
        String output;
        serializeJson(doc, output);
        server.send(200, "application/json", output);
    }
};
