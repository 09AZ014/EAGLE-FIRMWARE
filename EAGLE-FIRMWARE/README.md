EAGLE-FIRMWARE
===============

Firmware modular para ESP32 com suporte dual de build:

- Arduino IDE (pasta `arduino/`)
- PlatformIO (pasta `platformio/`)

Branch ativa: `feat/m5stickc-plus2-dualbuild`

Funcionalidades principais
- Módulos independentes (ModuleManager/IModule)
- Wi‑Fi Manager (STA/AP + portal de configuração)
- Storage (LittleFS) para `config.json`
- REST API local: `/api/status`, `/api/config` (GET/POST)
- OTA (ArduinoOTA) protegido por token
- Safe Mode (token + botão físico na M5StickC Plus 2)
- Display (LovyanGFX/M5) com status básico
- IMU (aceleração/giroscópio) com logs e pronto para UI
- Microfone (placeholder para bargraph)
- Buzzer (alertas)
- Self Test no arranque (som/estado básico)
- Mini Web UI servida em `/ui`

Compatibilidade de boards
- ESP32 DevKit
- M5StickC Plus 2

Estrutura
- Consulte `docs/architecture.md` e `docs/boards.md`.

Guia rápido de build
- Arduino IDE: abra `arduino/EagleFirmware.ino` e compile.
- PlatformIO: `cd platformio && pio run -e esp32dev` ou `-e m5stickc_plus2`.

Tutorial detalhado — PlatformIO
1) Instalação
   - Instale o PlatformIO: `pip install platformio` ou a extensão PlatformIO IDE no VS Code.
2) Seleção de ambiente
   - ESP32 DevKit: `-e esp32dev`
   - M5StickC Plus 2: `-e m5stickc_plus2`
3) Compilar
   - `pio run -e m5stickc_plus2`
4) Upload do firmware
   - Conecte a placa (porta COM/tty correta) e execute: `pio run -e m5stickc_plus2 -t upload`
5) Upload do sistema de ficheiros (LittleFS)
   - `pio run -e m5stickc_plus2 -t uploadfs`
6) Monitor série
   - `pio device monitor -b 115200`
7) UI e API
   - Aceda `http://<ip>/ui`
   - API: `http://<ip>:8080/api/status` e `http://<ip>:8080/api/config`

Tutorial detalhado — Arduino IDE
1) Preparação
   - Arduino IDE 2.x instalado
   - Core ESP32 3.x instalado (Boards Manager: Espressif ESP32)
   - Bibliotecas recomendadas: `M5StickCPlus2`, `LovyanGFX`, `ArduinoJson`
2) Abrir o projeto
   - `File > Open...` selecione `arduino/EagleFirmware.ino`
3) Selecionar a placa
   - ESP32 Dev Module (ou M5StickC Plus 2 se listado)
4) Compilar e enviar
   - Clique em `Verify` e depois `Upload`
5) Monitor série
   - 115200 baud

Notas
- OTA requer `api_token` definido em `config.json` via portal ou REST.
- Safe Mode: manter o Botão A pressionado na M5StickC Plus 2 ao arrancar.
- Para servir a Mini UI, o `index.html` é colocado em LittleFS via `uploadfs` no PlatformIO.


