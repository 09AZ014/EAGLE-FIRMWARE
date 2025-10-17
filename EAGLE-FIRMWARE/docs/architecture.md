# Arquitetura

- Versão Arduino: simples, `setup()/loop()`, headers em `arduino/`.
- Versão PlatformIO: modular, `src/modules`, `src/core`, `src/ui`.
- `board_config.h` centraliza a seleção de board.
- Módulos sugeridos: wifi, display, imu, buzzer, storage, ota, api.

