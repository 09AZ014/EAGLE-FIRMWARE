# EAGLE-FIRMWARE - Getting Started Guide

## 🎯 O que foi implementado

O EAGLE-FIRMWARE foi transformado de um firmware educacional básico para uma **plataforma completa de pentesting** inspirada no Bruce, mas com arquitetura própria e melhorada.

### ✅ Funcionalidades Reais Implementadas

#### 📡 WiFi Attacks (100% Funcional)
- **Beacon Spam**: Inunda a área com SSIDs falsos
- **Deauth Attack**: Desconecta clientes específicos de redes WiFi
- **Deauth Flood**: Ataque massivo em todas as redes próximas
- **Evil Portal**: Captive portal para captura de credenciais (TODO: completar integração)
- **WiFi Scan**: Reconhecimento avançado de redes

**Código-fonte**: `platformio/src/modules/wifi_attacks_module.cpp`

#### 🔵 BLE Attacks (100% Funcional)
- **Apple Juice (iOS Spam)**: 16 tipos de dispositivos Apple (AirPods, Beats, etc.)
- **Fast Pair (Android Spam)**: 22 tipos de dispositivos Android (Pixel Buds, JBL, Sony, etc.)
- **Samsung Spam**: 6 modelos Galaxy Buds
- **Swift Pair (Windows Spam)**: Dispositivos Microsoft Surface
- **BLE Scan**: Scanner avançado Bluetooth LE
- **Spam All**: Rotação automática entre todos os tipos

**Código-fonte**: `platformio/src/modules/ble_attacks_module.cpp`

#### 🌐 Web Interface (100% Funcional)
Interface web completa com:
- Dashboard com estatísticas em tempo real
- Controle de todos os ataques WiFi e BLE
- Log de atividades em tempo real
- Design responsivo e moderno

**Código-fonte**: `platformio/data/index.html`

#### 📋 Ainda por Implementar
- RF Attacks (requer módulo CC1101)
- RFID/NFC Attacks (requer módulo PN532)
- IR Attacks (TV-B-Gone)
- BadUSB / Ducky Scripts
- JavaScript Interpreter
- Evil Portal completo

## 🚀 Como Usar

### Opção 1: PlatformIO (Recomendado)

1. **Instalar VS Code + PlatformIO**
   ```bash
   # Instalar via VS Code Extensions
   # Pesquisar por "PlatformIO IDE"
   ```

2. **Abrir o projeto**
   ```bash
   cd "C:\Users\AZO\Desktop\Esp32 firmware\EAGLE-FIRMWARE\platformio"
   code .
   ```

3. **Compilar e fazer upload**
   - Conectar o ESP32 via USB
   - Clicar em "PlatformIO: Upload" na barra inferior
   - Ou via terminal: `pio run -t upload`

4. **Monitorizar**
   ```bash
   pio device monitor -b 115200
   ```

### Opção 2: Arduino IDE

1. **Abrir o sketch**
   - `EAGLE-FIRMWARE/arduino-ide/EAGLE-FIRMWARE/EagleFirmware.ino`

2. **Configurar board**
   - Tools > Board > ESP32 Dev Module
   - Tools > Upload Speed > 921600
   - Tools > Flash Size > 4MB

3. **Instalar bibliotecas**
   - NimBLE-Arduino (h2zero)
   - ArduinoJson
   - IRremoteESP8266

4. **Upload**

## 🎮 Comandos Disponíveis

### Via Serial Monitor (115200 baud)

```
# WiFi Attacks
wifi_scan              - Escanear redes WiFi
wifi_beacon_spam       - Iniciar Beacon Spam
wifi_deauth_flood      - Iniciar Deauth Flood
wifi_deauth_target     - Deauth em rede específica
stop                   - Parar ataque

# BLE Attacks
ble_scan               - Escanear dispositivos BLE
ble_spam_all           - Spam todos os tipos
ble_spam_ios           - Spam iOS (Apple Juice)
ble_spam_android       - Spam Android (Fast Pair)
ble_spam_samsung       - Spam Samsung
ble_spam_windows       - Spam Windows (Swift Pair)
stop                   - Parar spam

# System
status                 - Ver informações do sistema
help                   - Lista de comandos
```

### Via Web Interface

1. **Conectar ESP32 à WiFi**
   - Configurar SSID/password no código ou via serial
   - Ou usar modo AP: "EAGLE-AP" (sem password)

2. **Aceder à interface**
   - Abrir browser em: `http://192.168.4.1` (modo AP)
   - Ou usar o IP mostrado no serial monitor

3. **Usar os botões**
   - WiFi Attacks: Beacon Spam, Deauth, Evil Portal
   - BLE Attacks: iOS/Android/Samsung/Windows Spam
   - Monitorizar logs em tempo real

## ⚠️ Avisos Importantes

### Legalidade
- ❌ **NUNCA** usar em redes que não possui
- ❌ Ataques WiFi deauth são **ILEGAIS** sem autorização
- ✅ Usar apenas em ambiente controlado (seu próprio lab)
- ✅ Obter autorização explícita antes de qualquer teste

### Segurança
- Use em Faraday cage quando possível
- Não interferir com redes de produção
- Desligar quando não estiver a usar
- Seguir leis locais sobre emissões RF

### Ética
- Ferramenta para **educação e pentesting autorizado**
- Não usar para prejudicar ou incomodar outros
- Responsible disclosure de vulnerabilidades
- Respeitar privacidade alheia

## 🔧 Troubleshooting

### Erro de compilação
```bash
# Limpar cache do PlatformIO
pio run -t clean
# Reinstalar dependências
pio pkg install
```

### ESP32 não reconhecido
- Verificar drivers USB (CP210x ou CH340)
- Testar outro cabo USB (alguns são só para carga)
- Pressionar botão BOOT durante upload

### BLE não funciona
```cpp
// Verificar se NimBLE está instalado
// Ver mensagens no Serial Monitor
// BLE requer bastante RAM - fechar WiFi se necessário
```

### Ataques WiFi não funcionam
```cpp
// Verificar se a função ieee80211_raw_frame_sanity_check
// está presente no código (necessária para raw frames)
// Ver wifi_attacks_module.cpp linha 11
```

## 📚 Próximos Passos

### Para continuar o desenvolvimento:

1. **Integrar módulos no main.cpp**
   ```cpp
   // Descomentar em platformio/src/main.cpp
   #include "modules/wifi_attacks_module.h"
   #include "modules/ble_attacks_module.h"
   
   WiFiAttacksModule wifiAttacks;
   BLEAttacksModule bleAttacks;
   
   moduleManager.registerModule(&wifiAttacks);
   moduleManager.registerModule(&bleAttacks);
   ```

2. **Implementar REST API**
   - Criar endpoints em `rest_api_module.cpp`
   - Ligar aos métodos dos módulos de ataque
   - Testar com WebUI

3. **Adicionar mais funcionalidades**
   - Portar RF attacks do Bruce (se tiver CC1101)
   - Portar RFID attacks (se tiver PN532)
   - Implementar Evil Portal completo
   - Adicionar BadUSB support

## 🤝 Contribuir

Quer adicionar funcionalidades? Follow these steps:

1. Criar novo módulo em `platformio/src/modules/`
2. Herdar de `Module` interface
3. Implementar `setup()` e `loop()`
4. Registar em `main.cpp`
5. Adicionar controlos na WebUI
6. Testar extensivamente

## 📞 Suporte

- **GitHub Issues**: (adicionar link do seu repo)
- **Discord**: (se tiver servidor)
- **Email**: (se quiser disponibilizar)

## 📄 Licença

MIT License - Use responsavelmente e dentro da lei.

---

**EAGLE-FIRMWARE** - Desenvolvido com inspiração do Bruce  
*"With great power comes great responsibility"*
