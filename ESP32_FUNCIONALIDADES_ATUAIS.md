# ESP32 - Funcionalidades Atuais Implementadas

## 📅 Backup Criado
- **Data:** 21/10/2025 22:47:56
- **Arquivo Backup:** `src/main_backup_20251021_224756.cpp`

## 🔧 Hardware Configurado
- **ESP32 Dev Module**
- **LEDs:** WS2812B na porta 2 (16 LEDs)
- **Botões:** Porta 4 (Player 1), Porta 5 (Player 2)
- **WiFi:** Access Point "CoralVivoVR" (senha: 12345678)
- **WebSocket:** Porta 80

## 🎮 Mapa de LEDs
```
Player 1: LEDs 8-15 (sequência inversa: 15→14→13...→8)
Player 2: LEDs 0-7 (sequência normal: 0→1→2...→7)
Ready Player 1: LED 15 (verde)
Ready Player 2: LED 7 (verde)
```

## 📡 Comandos WebSocket Implementados

### 📥 MENSAGENS RECEBIDAS (do Unity)
- `vr_connected1` / `vr_connected2` - Unity conectado, enviar `ready1`/`ready2`
- `percent1:X` - Progresso Player 1 (0-100%)
- `percent2:X` - Progresso Player 2 (0-100%)
- `video_ended1` / `video_ended2` - Vídeo terminou, limpar timeline e mostrar ready

### 📤 MENSAGENS ENVIADAS (para o Unity)
- `button1` / `button2` - Botão físico pressionado (clique rápido <2s)
- `long1` / `long2` - Botão pressionado por ≥2s (stop completo)
- `ready1` / `ready2` - Player pronto/aguardando

### ⚠️ COMANDOS OBSOLETOS (não usar):
- `play1`/`play2`, `pause1`/`pause2`, `stop1`/`stop2` - Substit wido por `button`/`long`
- `vr_disconnected1`/`2` - ESP32 detecta automaticamente desconexão
- `red1`/`2`, `blue1`/`2`, `green1`/`2`, `orange1`/`2` - Cores controladas automaticamente

### 🎯 LÓGICA DO SISTEMA DUAL-PLAYER
1. Unity envia `vr_connected{X}` → ESP32 responde com `ready{X}`
2. Unity envia `percent{X}:Y` → ESP32 acende LEDs correspondentes ao progresso
3. Botão físico pressionado → ESP32 envia `button{X}` para Unity
4. Botão long press (≥2s) → ESP32 envia `long{X}` para Unity
5. Unity termina vídeo → Unity envia `video_ended{X}` → ESP32 limpa timeline

## ⚡ Funcionalidades Principais

### 1. Sistema de Botões
- **Debounce:** 20ms
- **Quick Click:** < 2s (PLAY/PAUSE)
- **Long Press:** ≥ 2s (STOP)
- **Detecção:** LOW quando pressionado

### 2. Progressão de LEDs
- **Fórmula:** `fullLEDs = (percent * 8) / 100`
- **Intensidade:** 20% → 100% por LED
- **Sequência:** Player 1 (inversa), Player 2 (normal)

### 3. Estados VR
- **Conexão:** LED ready verde
- **Desconexão/Hibernação:** LED laranja piscante
- **Independente:** Cada player tem seu estado

### 4. WebSocket Server
- **IP:** 192.168.0.1
- **Porta:** 80
- **Mensagens:** `ready1`, `ready2` na conexão
- **Parsing:** `msg.trim()` para remover whitespace

## 🚀 Próximas Funcionalidades Sugeridas
- [ ] Sistema de configuração via web interface
- [ ] Logs de eventos em SD card
- [ ] Calibração automática de LEDs
- [ ] Modo de demonstração automático
- [ ] Integração com sensores de movimento
- [ ] Sistema de backup de configurações
- [ ] Modo de economia de energia
- [ ] Comunicação com múltiplos dispositivos VR

## 📝 Notas Técnicas
- **Framework:** Arduino ESP32
- **Libraries:** FastLED, WebSockets, WiFi, ArduinoJson
- **Upload Speed:** 115200 bps
- **Monitor Speed:** 115200 bps
- **Build Flags:** `-DARDUINO_USB_MODE=1`

---
*Documentação criada em: 21/10/2025*

