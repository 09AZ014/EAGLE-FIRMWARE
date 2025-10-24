# EAGLE-FIRMWARE – Compilação e Upload com PlatformIO

## Ambientes Suportados
- **esp32dev** (ESP32 normal)
- **m5stickc_plus2** (M5 Stick C Plus 2)

---

## Pré‑requisitos
- **Python 3.x** instalado
- **PlatformIO CLI** (instale com `pip install platformio` ou `pipx install platformio`)
- **Drivers USB/Serial** para sua placa (ex: CP210x, CH340 etc.)
- Cabo USB de dados (não apenas carregamento)

---

## Compilação do Firmware
1. **Entre na pasta do PlatformIO**:
   ```bash
   cd EAGLE-FIRMWARE/platformio
   ```
2. **Compile para ESP32 normal:**
   ```bash
   python -m platformio run -e esp32dev
   ```
3. **Compile para M5 Stick C Plus 2:**
   ```bash
   python -m platformio run -e m5stickc_plus2
   ```
---

## Upload do Firmware (Gravação / Flash)
### 1. Certifique-se de que a placa está conectada corretamente via USB.
### 2. Descubra sua porta serial (em Windows, costuma ser COM3, COM4 etc.; no Linux/Mac, /dev/ttyUSB0 ou /dev/ttyACM0).

#### ESP32 normal:
```bash
python -m platformio run -t upload -e esp32dev
# ou especificando a porta:
python -m platformio run -t upload -e esp32dev --upload-port COM4
```

#### M5 Stick C Plus 2:
```bash
python -m platformio run -t upload -e m5stickc_plus2
# ou especificando a porta:
python -m platformio run -t upload -e m5stickc_plus2 --upload-port COM4
```

---

## Upload do Sistema de Ficheiros (LittleFS ou SPIFFS)
Se seu firmware usa arquivos web ou configurações salvos em LittleFS:
```bash
python -m platformio run -t uploadfs -e esp32dev
python -m platformio run -t uploadfs -e m5stickc_plus2
```

---

## Monitor Serial
Para acompanhar logs, mensagens ou interagir com o firmware:
```bash
python -m platformio device monitor -e esp32dev
# ou
python -m platformio device monitor --port COM4 --baud 115200
```
> Dica: use sempre a mesma velocidade definida em `platformio.ini` (normalmente 115200).

---

## Resolvendo Possíveis Erros
- **Permissões na porta (Linux/Mac):**
  - Adicione seu usuário ao grupo `dialout` e reinicie o sistema, se necessário.
- **Problemas de driver:**
  - Use o Gerenciador de Dispositivos (Windows) ou `dmesg | grep tty` (Linux) para verificar se a placa foi reconhecida.
- **Permissão negada:**
  - Execute o terminal com permissões de administrador (Windows) ou use `sudo` (Linux – mas evite sempre que possível).
- **Erro PATH após instalar PlatformIO:**
  - Tente sempre rodar via `python -m platformio ...`, pois isso garante o uso dos scripts no local correto.

---

## Estrutura do Projeto
- `src/core/` – arquivos centrais (configuração, gerenciamento de módulos)
- `src/modules/` – módulos principais (Wi-Fi, OTA, REST API, IMU, Display etc.)
- `src/boards/` – configurações específicas por board
- `data/` – arquivos para LittleFS (Mini Web UI)
- `platformio.ini` – configuração de ambientes, dependências e flags

---

## Dúvidas?
Abra um issue ou consulte o README principal.

