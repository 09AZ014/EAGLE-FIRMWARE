#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "core/module_manager.h"
#include "modules/storage_module.h"

class RestApiModule : public IModule {
public:
	RestApiModule(StorageModule& storage) : storage(storage), server(8080) {}
	const char* getName() const override { return "rest_api"; }
	void setup() override {
		server.on("/api/status", HTTP_GET, [this]() { onStatus(); });
		server.on("/api/config", HTTP_GET, [this]() { onConfigGet(); });
		server.on("/api/config", HTTP_POST, [this]() { onConfigPost(); });
		server.begin();
	}
	void loop() override { server.handleClient(); }

private:
	bool isAuthorized() {
		String token = server.hasHeader("X-API-Token") ? server.header("X-API-Token") : "";
		return storage.apiToken.length() > 0 && token == storage.apiToken;
	}

	void onStatus() {
		DynamicJsonDocument doc(512);
		doc["board"] = EAGLE_BOARD_NAME;
		doc["ip"] = WiFi.isConnected() ? WiFi.localIP().toString() : String("");
		String out; serializeJson(doc, out);
		server.send(200, "application/json", out);
	}

	void onConfigGet() {
		if (!isAuthorized()) { server.send(403, "text/plain", "Forbidden"); return; }
		DynamicJsonDocument doc(512);
		doc["wifi_ssid"] = storage.wifiSsid;
		String out; serializeJson(doc, out);
		server.send(200, "application/json", out);
	}

	void onConfigPost() {
		if (!isAuthorized()) { server.send(403, "text/plain", "Forbidden"); return; }
		if (!server.hasArg("plain")) { server.send(400, "text/plain", "Bad Request"); return; }
		DynamicJsonDocument doc(512);
		auto err = deserializeJson(doc, server.arg("plain"));
		if (err) { server.send(400, "text/plain", "Invalid JSON"); return; }
		String ssid = doc["wifi_ssid"].as<String>();
		String pass = doc["wifi_pass"].as<String>();
		String token = doc["api_token"].as<String>();
		bool ok = storage.saveConfig(ssid, pass, token);
		server.send(ok ? 200 : 500, "text/plain", ok ? "OK" : "ERROR");
	}

	StorageModule& storage;
	WebServer server;
};


