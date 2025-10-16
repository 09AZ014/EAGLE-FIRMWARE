#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NimBLEDevice.h>

// OLED
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_ADDR 0x3C

// Botões (ajuste conforme seu hardware)
#define BTN_UP     32
#define BTN_DOWN   33
#define BTN_BACK   25
#define BTN_OK     26

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

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

void drawTitle(const char *title) {
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(SSD1306_WHITE);
	display.setCursor(0, 0);
	display.println(title);
}

void drawMenu() {
	drawTitle("Menu");
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
	// Evita escanear a cada loop: a cada 3s
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

	IPAddress ip = WiFi.localIP();
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

	WiFi.mode(WIFI_STA);
	WiFi.disconnect(true, true);

	NimBLEDevice::init("");
	bleScan = NimBLEDevice::getScan();
}

void loop() {
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

	delay(50);
}
