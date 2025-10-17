# PlatformIO

## Ambientes
- esp32dev
- m5stickc_plus2

## Pré‑requisitos
- Python 3.x e PlatformIO CLI (`pip install platformio`)
- Drivers USB/serial da sua placa (CP210x/CH340, dependendo do board)

## Build / Upload
```bash
# Compilar
pio run -e esp32dev
pio run -e m5stickc_plus2

# Upload do firmware
pio run -e m5stickc_plus2 -t upload

# Upload do sistema de ficheiros (LittleFS)
pio run -e m5stickc_plus2 -t uploadfs

# Monitor série
pio device monitor -b 115200
```

## Estrutura
- `src/core/` – `board_config.h`, `module_manager.h`
- `src/modules/` – módulos (wifi, storage, api, ota, display, imu, mic, buzzer, self_test)
- `src/boards/` – definições específicas por board
- `data/` – ficheiros servidos via LittleFS (Mini Web UI)

## Notas
- `platformio.ini` contém flags comuns e específicas por board.
- `board_build.filesystem = littlefs` já configurado.
- OTA utiliza o `api_token` definido via portal (`/` → Save) ou REST.

