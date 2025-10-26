# ESP32 - CoralVivo VR LED Controller

## 📋 Visão Geral

Sistema ESP32 para controle de LEDs WS2812B em aplicação VR, com comunicação WebSocket e controle via botões físicos.

## 🔧 Hardware

### Componentes
- **ESP32 Dev Module** (240MHz, 320KB RAM, 4MB Flash)
- **Fita LED WS2812B** (16 LEDs) conectada no pino 2
- **2 Micro Switches** (Pinos 4 e 5) com pull-up interno

### Conexões
```
ESP32 Pin 2  → LED Data (WS2812B)
ESP32 Pin 4  → Botão Player 1 (INPUT_PULLUP)
ESP32 Pin 5  → Botão Player 2 (INPUT_PULLUP)
```

## 🌐 WiFi e WebSocket

### Access Point
- **SSID**: `CoralVivoVR`
- **Senha**: `12345678`
- **IP**: `192.168.4.1`
- **Porta WebSocket**: `80`

### Comandos WebSocket

#### Controle de Sessão
- `play1` - Inicia sessão Player 1
- `pause1` - Pausa sessão Player 1
- `stop1` - Para sessão Player 1
- `play2` - Inicia sessão Player 2
- `pause2` - Pausa sessão Player 2
- `stop2` - Para sessão Player 2

#### Controle de Cores
- `red1/orange1/yellow1/green1/blue1/purple1` - Player 1
- `red2/orange2/yellow2/green2/blue2/purple2` - Player 2
- `clear1/clear2/clear` - Limpar LEDs

#### Status Recebidos
- `status1:0` a `status1:100` - Progresso Player 1 (a cada 5%)
- `status1:paused` - Player 1 pausado
- `status1:stopped` - Player 1 parado
- `status1:complete` - Player 1 completo
- `status2:0` a `status2:100` - Progresso Player 2 (a cada 5%)
- `status2:paused` - Player 2 pausado
- `status2:stopped` - Player 2 parado
- `status2:complete` - Player 2 completo

## 🎮 Controle de Botões

### Player 1 (Pino 4)
- **Clique rápido** (< 2s): PLAY/PAUSE
- **Segurar** (2+ segundos): STOP

### Player 2 (Pino 5)
- **Clique rápido** (< 2s): PLAY/PAUSE
- **Segurar** (2+ segundos): STOP

## 💡 Sistema de LEDs

### Mapeamento
```
LEDs 0-7:   Player 2 (Azul)
LEDs 8-15:  Player 1 (Vermelho)
```

### Estados Ready
- **Player 1 Ready**: LED 15 verde
- **Player 2 Ready**: LED 7 verde (último LED da timeline azul)

### Animações Progressivas
- **Player 1**: Sequência 15→14→13→12→11→10→9→8 (vermelho)
- **Player 2**: Sequência 7→6→5→4→3→2→1→0 (azul)
- **Duração**: 5 segundos para completar
- **Brilho**: Progressivo de 20% a 100%

## 📁 Estrutura de Arquivos

### Arquivos Principais
- `src/main.cpp` - Código principal da ESP32
- `platformio_led_test.ini` - Configuração do PlatformIO

### Dependências
```ini
lib_deps = 
    fastled/FastLED@^3.6.0
    bblanchon/ArduinoJson@^7.0.4
    links2004/WebSockets@^2.3.6
```

## 🚀 Compilação e Upload

### Pré-requisitos
- PlatformIO instalado
- ESP32 conectado via USB

### Comandos
```bash
# Compilar e fazer upload
pio run --project-conf platformio_led_test.ini --target upload

# Monitorar serial
pio device monitor --port /dev/cu.usbserial-110 --baud 115200
```

### Configurações de Upload
- **Velocidade**: 460800 bps
- **Protocolo**: esptool
- **Porta**: Auto-detectada

## 🔄 Fluxo de Funcionamento

### Inicialização
1. Configura botões com pull-up interno
2. Inicializa FastLED (16 LEDs, brilho 50)
3. Cria Access Point "CoralVivoVR"
4. Inicia WebSocket Server na porta 80
5. Define LEDs de ready (15 verde, 7 verde)

### Durante Sessão
1. Monitora botões continuamente
2. Atualiza progressão de LEDs a cada loop
3. Envia status via WebSocket a cada 5%
4. Processa comandos WebSocket recebidos

### Estados das Sessões
- **Inactive**: Pronto para iniciar
- **Active**: Progressão em andamento
- **Paused**: Progressão pausada
- **Complete**: Volta ao estado ready

## 🐛 Debug e Monitoramento

### Serial Output
- Status de inicialização
- Comandos WebSocket recebidos
- Estados dos botões
- Progresso das sessões

### LEDs de Status
- **Verde**: Player pronto
- **Azul/Vermelho**: Progressão ativa
- **Apagado**: Inativo

## 📝 Notas Técnicas

### Debounce
- **Delay**: 50ms para botões
- **Prevenção**: Flags para evitar múltiplos STOs

### Timing
- **Loop delay**: 10ms
- **Progressão**: 5 segundos = 100%
- **Status update**: A cada 5% de progresso

### Memória
- **RAM**: ~14.8% (48KB de 320KB)
- **Flash**: ~75.2% (985KB de 1.3MB)

## 🔧 Troubleshooting

### Problemas Comuns
1. **Upload falha**: Verificar velocidade de upload
2. **WebSocket não conecta**: Verificar IP (192.168.4.1:80)
3. **LEDs não acendem**: Verificar conexão no pino 2
4. **Botões não respondem**: Verificar pull-up interno

### Logs Importantes
- `CLIENTE CONECTADO!` - WebSocket conectado
- `*** SESSÃO X: Y% ***` - Progresso da sessão
- `BOTAO X PRESSIONADO!` - Botão detectado
- `*** WEBSOCKET MSG: comando ***` - Comando recebido
