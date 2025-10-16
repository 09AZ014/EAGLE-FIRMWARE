# EAGLE-FIRMWARE: Mapping Bruce to MVP components (Ético)

Este firmware replica a base do UI ético (Wi‑Fi/BLE passivo, Calculadora, Sistema) e estrutura a arquitetura em MVP (Model / View / Presenter), sem funcionalidades ofensivas.

## MVP

- Model (módulos e estado)
  - WiFi: varredura passiva (SSID/RSSI/canal)
  - BLE: varredura passiva (endereços/RSSI)
  - Configuração: parâmetros de UI, pinos, preferências (futuro: LittleFS)
  - Estado: ecrã atual, índice do menu, resultados de scans

- View (UI / apresentação)
  - OLED 0.96" 128x64 (I2C) com `Adafruit_SSD1306`
  - Menus e páginas: WiFi, BLE, Calculadora, Sistema
  - Leitura de botões (UP/DOWN/OK/BACK) como input do utilizador

- Presenter / Controller (orquestração)
  - Liga inputs da View (botões) aos métodos do Model
  - Garante intervalos de scan para não bloquear a UI
  - Atualiza a View com resultados (listas WiFi/BLE, cálculos, status)

## Fluxo de exemplo
1. Utilizador pressiona OK no menu WiFi.
2. Presenter muda o estado para `SCREEN_WIFI`.
3. Model executa `WiFi.scanNetworks()` (com cadência) e mantém resultados.
4. Presenter lê resultados do Model e manda a View renderizar no OLED.

## Hardware (por omissão)
- OLED SSD1306 I2C (0x3C)
- Botões: UP=GPIO32, DOWN=GPIO33, BACK=GPIO25, OK=GPIO26

Ajuste os GPIOs no `.ino` conforme o seu hardware.

## Bibliotecas
- Adafruit GFX Library
- Adafruit SSD1306
- NimBLE-Arduino

## Build (Arduino IDE)
1. Placa: ESP32 Dev Module
2. Instalar bibliotecas acima no Gestor de Bibliotecas
3. Abrir `EAGLE-FIRMWARE.ino`, compilar e carregar

Nota: Este firmware é exclusivamente para fins educativos e de monitorização passiva.
