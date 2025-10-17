# Arduino IDE

## Pré‑requisitos
- Arduino IDE 2.x
- Core ESP32 3.x (Boards Manager → Espressif ESP32)
- Bibliotecas recomendadas: `M5StickCPlus2`, `LovyanGFX`, `ArduinoJson`

## Passos
1) Abra `arduino/EagleFirmware.ino`
2) Selecione a placa
   - ESP32 Dev Module (ou M5StickC Plus 2 se disponível)
3) Porta serial correta (COM/tty)
4) Compile (`Verify`) e envie (`Upload`)
5) Monitor série a 115200 baud

## Notas
- O código Arduino usa `#ifdef ARDUINO` e mantém apenas o essencial.
- Headers específicos de board em `arduino/boards/`.

