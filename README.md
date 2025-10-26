# 🎮 BIJARI VR - Sistema de Vídeo 360° Dual-Player

Sistema completo de reprodução de vídeos 360° em VR com sincronização de LEDs via ESP32, suportando dois players independentes.

## 📋 Índice

- [Visão Geral](#visão-geral)
- [Arquitetura](#arquitetura)
- [Componentes](#componentes)
- [Instalação](#instalação)
- [Configuração](#configuração)
- [Uso](#uso)
- [Documentação](#documentação)

## 🎯 Visão Geral

Sistema de reprodução de vídeos 360° para Meta Quest 3 com:

- ✅ **Dual-Player Independente**: Dois players funcionando simultaneamente sem interferência
- ✅ **Sincronização LED**: ESP32 controla LEDs visuais de progresso para cada player
- ✅ **Controles WebSocket**: Comunicação bidirecional em tempo real
- ✅ **Modo Offline**: Funciona mesmo sem conexão ao ESP32
- ✅ **Armazenamento Externo**: Suporta vídeos do armazenamento do Quest 3

## 🏗️ Arquitetura

```
┌─────────────────┐       WebSocket       ┌──────────────┐
│   Quest 3 - 1   │ ◄──────────────────►  │              │
│  (User 1)       │                        │              │
│                 │                        │    ESP32     │
│   Unity VR      │                        │   + LEDs     │
│   VRManager     │                        │              │
└─────────────────┘                        └──────────────┘
                                                 ▲
┌─────────────────┐       WebSocket            │
│   Quest 3 - 2   │ ◄──────────────────────────┘
│  (User 2)       │
│                 │
│   Unity VR      │
│   VRManager     │
└─────────────────┘
```

## 📦 Componentes

### 1. Unity VR Client (`socket-client/`)

**Arquivo Principal:** `Assets/Scripts/VRManager.cs`

Componente Unity responsável por:
- Gerenciar reprodução de vídeos 360°
- Comunicar via WebSocket com ESP32
- Enviar timecodes de progresso
- Receber comandos de controle
- Sistema dual-player individualizado

**Dependências:**
- Meta XR SDK (Quest 3)
- FastLED (ESP32)
- WebSocketsServer (ESP32)

### 2. ESP32 Firmware (`src/main.cpp`)

**Arquivo Principal:** `src/main.cpp`

Firmware ESP32 responsável por:
- Receber mensagens dos dois players
- Controlar LEDs de progresso (16 LEDs)
- Enviar comandos de controle
- Sistema totalmente independente por player

**Especificações:**
- LED WS2812B (16 LEDs)
- 2 Botões físicos
- WebSocket Server
- WiFi AP

### 3. Web Player (Teste) (`web_player/`)

Interface web para testes e debug do sistema.

## 🚀 Instalação

### Pré-requisitos

1. **Unity Hub** (2022.3 LTS ou superior)
2. **Meta Quest 3** (SDK instalado)
3. **ESP32 Dev Module**
4. **PlatformIO** ou Arduino IDE
5. **FastLED Library**

### Passo a Passo

1. **Clone o repositório**
```bash
git clone https://github.com/seu-usuario/BIJARI_VR.git
cd BIJARI_VR
```

2. **Abra o projeto Unity**
- Abra `socket-client/` no Unity Hub
- Aguarde resolução de dependências

3. **Configure o ESP32**
```bash
cd src
pio run --target upload
```

4. **Adicione vídeos**
- Consulte `VIDEO_README.md` para instruções
- Coloque vídeos 360° MP4

## ⚙️ Configuração

### Unity - VRManager.cs

No Inspector do VRManager, configure:

```csharp
[Header("User Settings")]
public int userNumber = 1; // 1 ou 2

[Header("Networking")]
public string serverUri = "ws://192.168.4.1:80";

[Header("Video Settings")]
public VideoPlayer videoPlayer;
public string[] videoFiles = { "seu_video.mp4" };
```

### ESP32 - Arduino/PlatformIO

No `platformio.ini`:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino

lib_deps = 
    fastled/FastLED@^3.6.0
    bblanchon/ArduinoJson@^6.21.3
    links2004/WebSockets@^2.3.6
```

### WiFi do ESP32

O ESP32 funciona como **Access Point** (não se conecta a WiFi existente).

**Configuração atual:**
```cpp
WiFi.softAP("CoralVivoVR", "12345678");
```

**Credenciais padrão:**
- **SSID:** `CoralVivoVR`
- **Senha:** `12345678`
- **IP:** `192.168.4.1`
- **Porta WebSocket:** `80`

Para alterar, edite `src/main.cpp` linha ~60.

## 🎮 Uso

### Build Dual-Player

1. **Build Player 1:**
   - Configure `userNumber = 1`
   - Build & Run
   - Conecte ao ESP32

2. **Build Player 2:**
   - Configure `userNumber = 2`
   - Build & Run
   - Conecte ao ESP32

### Controles

**Player 1 (LEDs 8-15):**
- Botão físico 1: Toggle Play/Pause
- Long press botão 1: Stop completo
- LEDs vermelhos mostram progresso

**Player 2 (LEDs 0-7):**
- Botão físico 2: Toggle Play/Pause
- Long press botão 2: Stop completo
- LEDs azuis mostram progresso

### Estados dos LEDs

- 🟢 **Verde (LED 15 ou 7):** Ready/Aguardando
- 🔴/🔵 **Vermelho/Azul (Progressivo):** Reproduzindo
- 🟠 **Laranja (Piscando):** Perda de conexão
- ⚫ **Apagado:** Offline/Desconectado

## 📚 Documentação

- **VRManager.cs**: `VIDEO_README.md` - Documentação completa do sistema Unity
- **ESP32**: `ESP32_DOCUMENTATION.md` - Detalhes do firmware
- **LEDs**: `LED_TEST_README.md` - Teste de LEDs
- **Upload**: `INSTRUCOES_UPLOAD_MANUAL.md` - Instruções de deploy

## 🔧 Debug

### Modo Diagnóstico

No Unity, ative:
```csharp
public bool diagnosticMode = true;
```

Logs detalhados aparecerão no console.

### Monitor ESP32

```bash
pio device monitor
```

### Teste de LEDs

```bash
cd src
pio run -e led_test -t upload
```

## 🌐 Redes

- **ESP32 WiFi AP:** `ws://192.168.4.1:80` (padrão)
- **Quest 3:** Conecta ao ESP32 automaticamente

## 📝 Mensagens

### Unity → ESP32
- `vr_connected1/2` - Conexão estabelecida
- `percent1/2:X` - Progresso (1-100)
- `video_ended1/2` - Vídeo terminou

### ESP32 → Unity
- `button1/2` - Botão pressionado
- `long1/2` - Botão longo
- `ready1/2` - Estado ready
- `vr_signal_lost1/2` - Perda de sinal

## ⚠️ Avisos Importantes

- ⚠️ Vídeos não estão no repositório (consulte `VIDEO_README.md`)
- ⚠️ Configure userNumber diferente para cada build
- ⚠️ Sistema é 100% individualizado entre players
- ⚠️ Não altere userNumber em runtime

## 📄 Licença

Este projeto é proprietário.

## 👥 Autores

Sistema BIJARI VR - 2024

## 🤝 Contribuindo

Este é um projeto fechado. Para suporte, contacte a equipe.

---

**Versão:** 2.0.0  
**Última Atualização:** Outubro 2024  
**Compatibilidade:** Meta Quest 3, ESP32, Unity 2022.3+
README test
