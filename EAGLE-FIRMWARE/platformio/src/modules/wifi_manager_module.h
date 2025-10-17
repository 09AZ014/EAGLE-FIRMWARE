#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "core/module_manager.h"
#include "modules/storage_module.h"

class WifiManagerModule : public IModule {
public:
	WifiManagerModule(StorageModule& storage) : storage(storage), server(80), apMode(false) {}
	const char* getName() const override { return "wifi_manager"; }
	void setup() override {
		if (storage.configLoaded && storage.wifiSsid.length()) {
			WiFi.mode(WIFI_STA);
			WiFi.begin(storage.wifiSsid.c_str(), storage.wifiPass.c_str());
			if (!waitForConnect(8000)) {
				startAP();
			}
		} else {
			startAP();
		}
		setupPortal();
	}
	void loop() override {
		server.handleClient();
	}

private:
	bool waitForConnect(unsigned long timeoutMs) {
		unsigned long start = millis();
		while (millis() - start < timeoutMs) {
			if (WiFi.status() == WL_CONNECTED) return true;
			delay(100);
		}
		return false;
	}

	void startAP() {
		WiFi.mode(WIFI_AP);
		WiFi.softAP("EAGLE-SETUP");
		apMode = true;
	}

	void setupPortal() {
		server.on("/", [this]() {
			String html = "<html><body><h3>EAGLE WiFi Manager</h3><form method='POST' action='/save'>SSID:<input name=ssid><br>PASS:<input name=pass><br>TOKEN:<input name=token><br><button>Save</button></form></body></html>";
			server.send(200, "text/html", html);
		});
		server.on("/save", HTTP_POST, [this]() {
			String ssid = server.arg("ssid");
			String pass = server.arg("pass");
			String token = server.arg("token");
			storage.saveConfig(ssid, pass, token);
			server.send(200, "text/plain", "Saved. Rebooting...");
			delay(500);
			ESP.restart();
		});
		server.begin();
	}

	StorageModule& storage;
	WebServer server;
	bool apMode;
};


