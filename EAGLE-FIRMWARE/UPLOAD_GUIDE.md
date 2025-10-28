# 🚀 Guia de Upload para M5StickC Plus 2

## Método 1: PlatformIO via VS Code (RECOMENDADO)

### Passo 1: Instalar VS Code
1. Download: https://code.visualstudio.com/
2. Instalar normalmente
3. Abrir VS Code

### Passo 2: Instalar PlatformIO Extension
1. No VS Code, clicar no ícone de Extensions (ou `Ctrl+Shift+X`)
2. Procurar por "PlatformIO IDE"
3. Clicar em "Install"
4. Aguardar instalação completa (pode demorar alguns minutos)
5. **Reiniciar VS Code**

### Passo 3: Abrir o Projeto
1. No VS Code: `File > Open Folder`
2. Navegar para: `C:\Users\AZO\Desktop\Esp32 firmware\EAGLE-FIRMWARE\platformio`
3. Abrir esta pasta

### Passo 4: Conectar M5StickC Plus 2
1. Ligar o M5StickC Plus 2 ao PC via USB-C
2. Windows irá instalar drivers automaticamente
3. Verificar em "Device Manager" > "Ports (COM & LPT)" qual é a porta COM

### Passo 5: Upload para M5StickC Plus 2
1. Na barra inferior do VS Code, encontrar o ícone PlatformIO
2. Clicar na seta ao lado de "m5stickc_plus2"
3. Clicar em "Upload"
4. **OU** abrir PlatformIO menu (ícone alien/casa) e selecionar:
   - Project Tasks > m5stickc_plus2 > General > Upload

### Passo 6: Monitor Serial
1. Após upload, abrir Serial Monitor:
   - PlatformIO menu > Project Tasks > m5stickc_plus2 > Monitor
2. Velocidade: 115200 baud (já configurado)
3. Ver mensagens do EAGLE-FIRMWARE

---

## Método 2: PlatformIO CLI (Terminal)

### Passo 1: Instalar Python
1. Download: https://www.python.org/downloads/
2. **IMPORTANTE:** Marcar "Add Python to PATH" durante instalação

### Passo 2: Instalar PlatformIO Core
Abrir PowerShell e executar:
```powershell
pip install platformio
```

### Passo 3: Navegar para o Projeto
```powershell
cd "C:\Users\AZO\Desktop\Esp32 firmware\EAGLE-FIRMWARE\platformio"
```

### Passo 4: Upload
```powershell
# Compilar e fazer upload
pio run -e m5stickc_plus2 -t upload

# Se a porta COM não for detectada automaticamente:
pio run -e m5stickc_plus2 -t upload --upload-port COM3
# (substituir COM3 pela porta correta)
```

### Passo 5: Monitor Serial
```powershell
pio device monitor -b 115200
```

---

## Método 3: Arduino IDE (Alternativa Simples)

### Passo 1: Instalar Arduino IDE 2.x
1. Download: https://www.arduino.cc/en/software
2. Instalar normalmente

### Passo 2: Adicionar ESP32 Board Support
1. Abrir Arduino IDE
2. `File > Preferences`
3. Em "Additional Board Manager URLs", adicionar:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. `Tools > Board > Boards Manager`
5. Procurar "esp32" e instalar "esp32 by Espressif Systems"

### Passo 3: Instalar Bibliotecas
`Tools > Manage Libraries`, procurar e instalar:
- **NimBLE-Arduino** (by h2zero)
- **ArduinoJson** (by Benoit Blanchon)
- **IRremoteESP8266** (by David Conran)
- **M5StickCPlus2** (by M5Stack)

### Passo 4: Abrir Sketch
1. `File > Open`
2. Navegar para: `C:\Users\AZO\Desktop\Esp32 firmware\EAGLE-FIRMWARE\arduino\EagleFirmware\EagleFirmware.ino`

### Passo 5: Configurar Board
1. `Tools > Board > ESP32 Arduino > M5StickC Plus2`
2. `Tools > Port > [Selecionar porta COM do M5StickC]`
3. `Tools > Upload Speed > 921600`

### Passo 6: Upload
1. Clicar no botão "Upload" (seta para direita)
2. Aguardar compilação e upload
3. Abrir Serial Monitor (`Ctrl+Shift+M`) em 115200 baud

---

## 🔧 Resolução de Problemas

### Erro: "Port not found" / "Failed to connect"
**Solução:**
1. Instalar driver CH340/CP210x:
   - CH340: https://learn.sparkfun.com/tutorials/how-to-install-ch340-drivers/all
   - CP210x: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
2. Reiniciar PC
3. Verificar cabo USB-C (alguns são apenas para carregar)
4. Pressionar botão RESET no M5StickC enquanto faz upload

### Erro: "Compilation error"
**Solução:**
1. Limpar build: PlatformIO menu > Project Tasks > Clean
2. Tentar novamente
3. Verificar se todas as bibliotecas foram instaladas

### M5StickC não liga após upload
**Solução:**
1. Pressionar botão PWR (lateral) por 2 segundos
2. Ligar a cabo USB
3. Se tela ficar preta, é normal - usa Serial Monitor

### Monitor Serial não mostra nada
**Solução:**
1. Verificar baudrate (115200)
2. Pressionar botão RESET no M5StickC
3. Verificar se porta COM está correta

---

## 📱 Após Upload Bem-Sucedido

### Ver saída no Serial Monitor (115200 baud):
```
EAGLE-FIRMWARE (PlatformIO) starting...
Board: M5StickC Plus 2

========================================
   🦅 EAGLE-FIRMWARE v1.0
   Advanced ESP32 Pentesting Platform
========================================

✅ WiFi Attacks: ACTIVE
✅ BLE Attacks: ACTIVE
✅ IR Attacks: ACTIVE
✅ Evil Portal: ACTIVE
✅ Web Interface: ACTIVE

⚠️  USE ONLY ON YOUR OWN EQUIPMENT!
⚠️  UNAUTHORIZED USE IS ILLEGAL!

Type 'help' for available commands

========================================
```

### Comandos disponíveis:
```
help                   - Lista todos os comandos
wifi_beacon_spam       - Beacon spam attack
ble_spam_ios           - iOS BLE spam (Apple Juice)
evil_portal            - Captive portal
status                 - Ver status do sistema
stop                   - Parar ataques
```

---

## 🎯 Quick Start (Método Mais Rápido)

**Se tens Python instalado:**
```powershell
# 1. Instalar PlatformIO
pip install platformio

# 2. Navegar para pasta
cd "C:\Users\AZO\Desktop\Esp32 firmware\EAGLE-FIRMWARE\platformio"

# 3. Ligar M5StickC Plus 2 via USB

# 4. Upload
pio run -e m5stickc_plus2 -t upload

# 5. Monitor
pio device monitor -b 115200
```

---

## 📞 Suporte

Se tiveres problemas:
1. Verifica que o M5StickC está em modo bootloader (mantém botão enquanto liga)
2. Tenta porta COM diferente
3. Reinstala drivers USB
4. Usa cabo USB-C com **dados** (não apenas carga)

**Bom hacking! 🦅**
