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
#include "modules/self_test_module.h"
// Enhanced pentesting modules (temporarily disabled for basic build)
// #include "modules/port_scanner_module.h"
// #include "modules/vulnerability_scanner_module.h"
// #include "modules/pentest_web_interface_module.h"
// Bruce-inspired modules (temporarily disabled for basic build)
// #include "modules/wifi_advanced_module.h"
// #include "modules/ble_advanced_module.h"
// #include "modules/rf_advanced_module.h"
// #include "modules/rfid_advanced_module.h"
// #include "modules/ir_advanced_module.h"
// #include "modules/fm_advanced_module.h"
// #include "modules/nrf24_advanced_module.h"
// #include "modules/script_interpreter_module.h"
// #include "modules/espnow_module.h"
// #include "modules/utility_modules.h"

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
static SelfTestModule selfTest(buzzer);
// Enhanced pentesting modules (temporarily disabled)
// static PortScannerModule portScanner;
// static VulnerabilityScannerModule vulnScanner;
// static PentestWebInterfaceModule pentestWebInterface;
// Bruce-inspired modules (temporarily disabled)
// static WiFiAdvancedModule wifiAdvanced;
// static BLEAdvancedModule bleAdvanced;
// static RFAdvancedModule rfAdvanced;
// static RFIDAdvancedModule rfidAdvanced;
// static IRAdvancedModule irAdvanced;
// static FMAdvancedModule fmAdvanced;
// static NRF24AdvancedModule nrf24Advanced;
// static ScriptInterpreterModule scriptInterpreter;
// static ESPNowModule espnow;
// Utility modules (temporarily disabled)
// static MicrophoneSpectrumModule micSpectrum;
// static QRCodeModule qrCode;
// static SDCardManagerModule sdManager;
// static LittleFSManagerModule littlefsManager;
// static RTCModule rtc;
// static WebUIModule webUI;
// static PIXModule pix;

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
	moduleManager.registerModule(&selfTest);
	// Enhanced pentesting modules (temporarily disabled)
	// moduleManager.registerModule(&portScanner);
	// moduleManager.registerModule(&vulnScanner);
	// moduleManager.registerModule(&pentestWebInterface);
	// Bruce-inspired modules (temporarily disabled)
	// moduleManager.registerModule(&wifiAdvanced);
	// moduleManager.registerModule(&bleAdvanced);
	// moduleManager.registerModule(&rfAdvanced);
	// moduleManager.registerModule(&rfidAdvanced);
	// moduleManager.registerModule(&irAdvanced);
	// moduleManager.registerModule(&fmAdvanced);
	// moduleManager.registerModule(&nrf24Advanced);
	// moduleManager.registerModule(&scriptInterpreter);
	// moduleManager.registerModule(&espnow);
	// Utility modules (temporarily disabled)
	// moduleManager.registerModule(&micSpectrum);
	// moduleManager.registerModule(&qrCode);
	// moduleManager.registerModule(&sdManager);
	// moduleManager.registerModule(&littlefsManager);
	// moduleManager.registerModule(&rtc);
	// moduleManager.registerModule(&webUI);
	// moduleManager.registerModule(&pix);
	
	// Configure module relationships (temporarily disabled)
	// pentestWebInterface.setPortScanner(&portScanner);
	// pentestWebInterface.setVulnScanner(&vulnScanner);
	
	moduleManager.setupAll();
}

void loop() {
	moduleManager.loopAll();
	delay(50);
}


