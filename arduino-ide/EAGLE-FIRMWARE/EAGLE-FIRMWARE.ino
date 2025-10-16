#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NimBLEDevice.h>
#include <WebServer.h>
#include <LittleFS.h>

// OLED
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_ADDR 0x3C

// Botões (ajuste conforme seu hardware)
#define BTN_UP     32
#define BTN_DOWN   33
#define BTN_BACK   25
#define BTN_OK     26

// AP para Web UI
#define AP_SSID    "EAGLE-UI"
#define AP_PASS    "eagle1234" // mínimo 8 caracteres

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);
WebServer server(80);

// Debounce simples por botão
struct ButtonState {
	uint8_t pin;
	bool lastStableLow;
	uint32_t lastChangeMs;
};

ButtonState btnUp   { BTN_UP,   false, 0 };
ButtonState btnDown { BTN_DOWN, false, 0 };
ButtonState btnBack { BTN_BACK, false, 0 };
ButtonState btnOk   { BTN_OK,   false, 0 };

bool buttonPressed(ButtonState &b, uint16_t debounceMs = 120) {
	bool isLow = (digitalRead(b.pin) == LOW);
	uint32_t now = millis();
	if (isLow != b.lastStableLow && (now - b.lastChangeMs) > debounceMs) {
		b.lastChangeMs = now;
		b.lastStableLow = isLow;
		return isLow; // transição para pressionado (LOW)
	}
	return false;
}

// Estados de UI
enum AppScreen { SCREEN_MENU = 0, SCREEN_WIFI, SCREEN_BLE, SCREEN_CALC, SCREEN_SYS };
AppScreen currentScreen = SCREEN_MENU;
int menuIndex = 0;

// WiFi
uint32_t lastWifiScanMs = 0;
int wifiScanCount = 0;

// BLE
uint32_t lastBleScanMs = 0;
NimBLEScan *bleScan = nullptr;
NimBLEScanResults bleResults;

// Calculadora
long calcA = 0;
long calcB = 0;
char calcOp = '+';
bool calcEditingA = true;

// ---------------- Web UI (View via HTTP) e Presenter ----------------
String htmlIndex() {
	String h;
	h.reserve(3000);
	h += F("<!DOCTYPE html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>EAGLE-UI</title><style>body{font-family:sans-serif;margin:16px}pre{background:#111;color:#0f0;padding:8px;border-radius:6px;white-space:pre-wrap}button{margin:4px;padding:8px 12px}</style></head><body>");
	h += F("<h2>EAGLE-FIRMWARE (UI Ética)</h2><div><button onclick=scanWifi()>Scan WiFi</button><button onclick=scanBle()>Scan BLE</button><button onclick=loadStatus()>Status</button></div>");
	h += F("<h3>Saída</h3><pre id=out>Pronto.</pre>");
	h += F("<script>async function api(p){const r=await fetch(p);return r.ok?r.json():{error:'HTTP '+r.status}}\nasync function scanWifi(){out.textContent='A escanear WiFi...';const j=await api('/wifi/scan');out.textContent=JSON.stringify(j,null,2)}\nasync function scanBle(){out.textContent='A escanear BLE...';const j=await api('/ble/scan');out.textContent=JSON.stringify(j,null,2)}\nasync function loadStatus(){const j=await api('/status');out.textContent=JSON.stringify(j,null,2)}\n</script>");
	h += F("</body></html>");
	return h;
}

void handleRoot() {
	server.send(200, "text/html; charset=utf-8", htmlIndex());
}

void handleStatus() {
	// JSON manual
	String j = "{";
	j += "\"uptimeSec\":"; j += String(millis()/1000);
	j += ",\"heapFree\":"; j += String(ESP.getFreeHeap());
	IPAddress ip = WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP();
	j += ",\"ip\":\""; j += ip.toString(); j += "\"";
	j += ",\"screen\":"; j += String((int)currentScreen);
	j += "}";
	server.send(200, "application/json", j);
}

void handleWifiScanApi() {
	int n = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);
	String j = "[";
	for (int i = 0; i < n; i++) {
		if (i) j += ",";
		j += "{";
		j += "\"ssid\":\""; j += WiFi.SSID(i); j += "\",";
		j += "\"rssi\":"; j += String(WiFi.RSSI(i)); j += ",";
		#ifdef ESP32
		j += "\"channel\":"; j += String(WiFi.channel(i));
		#else
		j += "\"channel\":0";
		#endif
		j += "}";
	}
	j += "]";
	server.send(200, "application/json", j);
}

void handleBleScanApi() {
	if (!bleScan) bleScan = NimBLEDevice::getScan();
	bleScan->setActiveScan(false);
	NimBLEScanResults r = bleScan->start(3, false);
	String j = "[";
	for (int i = 0; i < r.getCount(); i++) {
		if (i) j += ",";
		NimBLEAdvertisedDevice d = r.getDevice(i);
		j += "{";
		j += "\"addr\":\""; j += d.getAddress().toString().c_str(); j += "\",";
		j += "\"rssi\":"; j += String((int)d.getRSSI());
		j += "}";
	}
	j += "]";
	server.send(200, "application/json", j);
}

void setupHttp() {
	server.on("/", HTTP_GET, handleRoot);
	server.on("/status", HTTP_GET, handleStatus);
	server.on("/wifi/scan", HTTP_GET, handleWifiScanApi);
	server.on("/ble/scan", HTTP_GET, handleBleScanApi);
	server.onNotFound([](){ server.send(404, "text/plain", "Not found"); });
	server.begin();
}

// ----------------- UI OLED -----------------
void drawTitle(const char *title) {
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(SSD1306_WHITE);
	display.setCursor(0, 0);
	display.println(title);
}

void drawMenu() {
	drawTitle("EAGLE-FIRMWARE");
	const char* items[] = { "WiFi Scan", "BLE Scan", "Calculadora", "Sistema" };
	const int itemCount = 4;
	for (int i = 0; i < itemCount; i++) {
		display.setCursor(0, 16 + i * 12);
		display.print((i == menuIndex) ? "> " : "  ");
		display.println(items[i]);
	}
	display.display();
}

void drawWifiScan() {
	drawTitle("WiFi Scan (passivo)");
	if (millis() - lastWifiScanMs > 3000) {
		wifiScanCount = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);
		lastWifiScanMs = millis();
	}

	int shown = min(wifiScanCount, 4);
	for (int i = 0; i < shown; i++) {
		display.setCursor(0, 16 + i * 12);
		display.print(WiFi.SSID(i));
		display.print(" ");
		display.print(WiFi.RSSI(i));
		display.print("dBm ");
		#ifdef ESP32
		display.print("ch");
		display.print(WiFi.channel(i));
		#endif
	}
	display.display();
}

void drawBleScan() {
	drawTitle("BLE Scan (passivo)");
	if (millis() - lastBleScanMs > 3000) {
		if (!bleScan) bleScan = NimBLEDevice::getScan();
		bleScan->setActiveScan(false);
		bleResults = bleScan->start(3, false);
		lastBleScanMs = millis();
	}

	int shown = min(bleResults.getCount(), 4);
	for (int i = 0; i < shown; i++) {
		NimBLEAdvertisedDevice d = bleResults.getDevice(i);
		display.setCursor(0, 16 + i * 12);
		display.print(d.getAddress().toString().c_str());
		display.print(" ");
		display.print((int)d.getRSSI());
		display.print("dBm");
	}
	display.display();
}

void drawCalc() {
	if (buttonPressed(btnUp))   { if (calcEditingA) calcA++; else calcB++; }
	if (buttonPressed(btnDown)) { if (calcEditingA) calcA = max(0L, calcA - 1); else calcB = max(0L, calcB - 1); }
	if (buttonPressed(btnOk))   { calcEditingA = !calcEditingA; }
	if (buttonPressed(btnBack)) {
		if      (calcOp == '+') calcOp = '-';
		else if (calcOp == '-') calcOp = '*';
		else if (calcOp == '*') calcOp = '/';
		else                    calcOp = '+';
	}

	long res = 0;
	switch (calcOp) {
		case '+': res = calcA + calcB; break;
		case '-': res = calcA - calcB; break;
		case '*': res = calcA * calcB; break;
		case '/': res = (calcB == 0 ? 0 : calcA / calcB); break;
	}

	drawTitle("Calculadora");
	display.setCursor(0, 20);
	display.print(calcEditingA ? "> " : "  ");
	display.print(calcA);
	display.print(" ");
	display.print(calcOp);
	display.print(" ");
	display.print(!calcEditingA ? "< " : "  ");
	display.println(calcB);

	display.setCursor(0, 40);
	display.print("= ");
	display.println(res);
	display.display();
}

void drawSys() {
	drawTitle("Sistema");
	display.setCursor(0, 16);
	display.print("Uptime: ");
	display.print(millis() / 1000);
	display.println("s");

	display.setCursor(0, 28);
	display.print("Heap: ");
	display.print(ESP.getFreeHeap());
	display.println(" B");

	IPAddress ip = WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP();
	display.setCursor(0, 40);
	display.print("IP: ");
	if (ip[0] == 0) display.println("N/A"); else display.println(ip);

	display.display();
}

void setupButtons() {
	pinMode(BTN_UP,   INPUT_PULLUP);
	pinMode(BTN_DOWN, INPUT_PULLUP);
	pinMode(BTN_BACK, INPUT_PULLUP);
	pinMode(BTN_OK,   INPUT_PULLUP);
}

void setup() {
	Serial.begin(115200);
	Wire.begin();
	setupButtons();

	if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
		while (true) { delay(1000); }
	}
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(SSD1306_WHITE);
	display.display();

	// FS
	LittleFS.begin(true);

	// WiFi: iniciar AP para Web UI
	WiFi.mode(WIFI_AP_STA);
	WiFi.softAP(AP_SSID, AP_PASS);

	// BLE
	NimBLEDevice::init("");
	bleScan = NimBLEDevice::getScan();

	// HTTP
	setupHttp();
}

void loop() {
	server.handleClient();

	if (currentScreen == SCREEN_MENU) {
		if (buttonPressed(btnUp))   menuIndex = (menuIndex + 3) % 4;
		if (buttonPressed(btnDown)) menuIndex = (menuIndex + 1) % 4;
		if (buttonPressed(btnOk))   currentScreen = (AppScreen)(menuIndex + 1);
		drawMenu();
	} else {
		if (buttonPressed(btnBack)) currentScreen = SCREEN_MENU;

		switch (currentScreen) {
			case SCREEN_WIFI: drawWifiScan(); break;
			case SCREEN_BLE:  drawBleScan();  break;
			case SCREEN_CALC: drawCalc();     break;
			case SCREEN_SYS:  drawSys();      break;
			default: break;
		}
	}

	delay(30);
}
