EAGLE-FIRMWARE
===============

Projeto de firmware modular para ESP32 com suporte a múltiplas placas e dois fluxos de build:

- Arduino IDE (pasta `arduino/`)
- PlatformIO (pasta `platformio/`)

Estado atual nesta branch: feat/m5stickc-plus2-dualbuild

Objetivos
- Suporte dual build (Arduino/PlatformIO)
- Modularização (módulos independentes)
- Suporte à M5 Stick C Plus 2
- CI no GitHub Actions para builds automáticos

Estrutura
- Consulte `docs/architecture.md` e `docs/boards.md`.

Build rápido
- Arduino IDE: abra `arduino/EagleFirmware.ino`.
- PlatformIO: `platformio run -e esp32dev` ou `-e m5stickc_plus2`.


