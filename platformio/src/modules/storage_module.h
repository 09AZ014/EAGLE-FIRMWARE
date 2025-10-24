#pragma once

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "core/module_manager.h"

class StorageModule : public IModule {
public:
	StorageModule() : configLoaded(false) {}
	const char* getName() const override { return "storage"; }
	void setup() override {
		LittleFS.begin(true);
		configLoaded = loadConfig();
	}
	void loop() override {}

	bool loadConfig() {
		File f = LittleFS.open("/config.json", "r");
		if (!f) return false;
		DynamicJsonDocument doc(1024);
		auto err = deserializeJson(doc, f);
		f.close();
		if (err) return false;
		wifiSsid = doc["wifi_ssid"].as<String>();
		wifiPass = doc["wifi_pass"].as<String>();
		apiToken = doc["api_token"].as<String>();
		return true;
	}

	bool saveConfig(const String& ssid, const String& pass, const String& token) {
		DynamicJsonDocument doc(1024);
		doc["wifi_ssid"] = ssid;
		doc["wifi_pass"] = pass;
		doc["api_token"] = token;
		File f = LittleFS.open("/config.json", "w");
		if (!f) return false;
		serializeJson(doc, f);
		f.close();
		wifiSsid = ssid; wifiPass = pass; apiToken = token;
		configLoaded = true;
		return true;
	}

	String wifiSsid;
	String wifiPass;
	String apiToken;
	bool configLoaded;
};


