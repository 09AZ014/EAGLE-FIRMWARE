#include <Arduino.h>

#include "core/board_config.h"
#include "core/module_manager.h"
#include "modules/wifi_module.h"
#include "modules/storage_module.h"
#include "modules/wifi_manager_module.h"
#include "modules/rest_api_module.h"
#include "modules/ota_module.h"
#include "modules/safe_mode_module.h"
#include "modules/display_module.h"
#include "modules/imu_module.h"
#include "modules/mic_module.h"
#include "modules/buzzer_module.h"

static ModuleManager moduleManager;
static StorageModule storage;
static SafeModeModule safeMode;
static WifiManagerModule wifiManager(storage);
static RestApiModule restApi(storage);
static OtaModule ota(storage);
static DisplayModule display;
static ImuModule imu;
static MicModule mic;
static BuzzerModule buzzer;

void setup() {
	Serial.begin(115200);
	delay(200);
	Serial.println("EAGLE-FIRMWARE (PlatformIO) starting...");
	Serial.printf("Board: %s\n", EAGLE_BOARD_NAME);

	moduleManager.registerModule(&safeMode);
	moduleManager.registerModule(&storage);
	moduleManager.registerModule(&wifiManager);
	moduleManager.registerModule(&restApi);
	moduleManager.registerModule(&ota);
	moduleManager.registerModule(&display);
	moduleManager.registerModule(&imu);
	moduleManager.registerModule(&mic);
	moduleManager.registerModule(&buzzer);
	moduleManager.setupAll();
}

void loop() {
	moduleManager.loopAll();
	delay(50);
}


