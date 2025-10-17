#pragma once

#include <Arduino.h>
#include <ArduinoOTA.h>
#include "core/module_manager.h"
#include "modules/storage_module.h"

class OtaModule : public IModule {
public:
	OtaModule(StorageModule& storage) : storage(storage) {}
	const char* getName() const override { return "ota"; }
	void setup() override {
		if (storage.apiToken.length() == 0) return; // only enable if token set
		ArduinoOTA.setPassword(storage.apiToken.c_str());
		ArduinoOTA.begin();
	}
	void loop() override { ArduinoOTA.handle(); }
private:
	StorageModule& storage;
};


