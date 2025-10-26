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

### Controle de Sessão
- `play1` / `play2` - Iniciar sessão
- `pause1` / `pause2` - Pausar sessão
- `stop1` / `stop2` - Parar sessão e voltar ao ready

### Controle de Percentual
- `percent1:X` - Player 1 (0-100%)
- `percent2:X` - Player 2 (0-100%)

### Estados VR por Player
- `vr_connected1` / `vr_connected2` - VR conectado
- `vr_disconnected1` / `vr_disconnected2` - VR desconectado
- `vr_hibernate1` / `vr_hibernate2` - VR hibernado
- `vr_signal_lost1` / `vr_signal_lost2` - Perda de sinal

### Cores Diretas
- `red1` / `red2` - LEDs vermelhos
- `blue1` / `blue2` - LEDs azuis
- `green1` / `green2` - LEDs verdes
- `orange1` / `orange2` - LEDs laranja
- `clear1` / `clear2` - Limpar LEDs do player
- `clear` - Limpar todos os LEDs

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

