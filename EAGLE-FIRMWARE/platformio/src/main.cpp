#include <Arduino.h>

#include "core/board_config.h"
#include "core/module_manager.h"
#include "modules/wifi_module.h"

static ModuleManager moduleManager;

void setup() {
	Serial.begin(115200);
	delay(200);
	Serial.println("EAGLE-FIRMWARE (PlatformIO) starting...");
	Serial.printf("Board: %s\n", EAGLE_BOARD_NAME);

	moduleManager.registerModule(new WifiModule());
	moduleManager.setupAll();
}

void loop() {
	moduleManager.loopAll();
	delay(50);
}


