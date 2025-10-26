# 🎮 Comandos VR para Teste via Aparato

## 📡 Conexão WebSocket
- **URL**: `ws://192.168.4.1:80`
- **Protocolo**: WebSocket

## 🎯 Comandos de VR para Simulação

### 🔌 Estados de Conexão
```
vr_connected      - Simula VR conectado (volta ao estado ready)
vr_disconnected   - Simula VR desconectado (ativa efeito laranja)
vr_hibernate      - Simula VR hibernado (botão power, efeito laranja)
vr_signal_lost    - Simula perda de sinal (timeout, efeito laranja)
```

### 🎮 Comandos de Sessão (Player 1)
```
play1             - Inicia sessão Player 1 (LEDs 8-15, vermelho)
pause1            - Pausa sessão Player 1
stop1             - Para sessão Player 1
```

### 🎮 Comandos de Sessão (Player 2)
```
play2             - Inicia sessão Player 2 (LEDs 0-7, azul)
pause2            - Pausa sessão Player 2
stop2             - Para sessão Player 2
```

### 🎨 Comandos de Cores (Player 1)
```
red1              - LED 15 vermelho
orange1           - LED 15 laranja
yellow1           - LED 15 amarelo
green1            - LED 15 verde
blue1             - LED 15 azul
purple1           - LED 15 roxo
```

### 🎨 Comandos de Cores (Player 2)
```
red2              - LED 7 vermelho
orange2           - LED 7 laranja
yellow2           - LED 7 amarelo
green2            - LED 7 verde
blue2             - LED 7 azul
purple2           - LED 7 roxo
```

### 🧹 Comandos de Limpeza
```
clear1            - Limpa LEDs Player 1 (8-15)
clear2            - Limpa LEDs Player 2 (0-7)
clear             - Limpa todos os LEDs
```

## 🧪 Sequência de Teste Recomendada

### 1. Teste de Conexão
```
vr_connected      - Deve mostrar LEDs 15 e 7 verdes
vr_disconnected   - Deve piscar todos os LEDs em laranja
vr_connected      - Deve voltar ao estado ready
```

### 2. Teste de Hibernação
```
play1             - Inicia sessão Player 1
vr_hibernate      - Deve parar sessão e piscar laranja
vr_connected      - Deve voltar ao estado ready
```

### 3. Teste de Perda de Sinal
```
play2             - Inicia sessão Player 2
vr_signal_lost    - Deve parar sessão e piscar laranja
vr_connected      - Deve voltar ao estado ready
```

### 4. Teste de Sessões
```
play1             - Player 1: progressão 15→14→13...→8 (vermelho)
play2             - Player 2: progressão 7→6→5...→0 (azul)
pause1            - Pausa Player 1
pause2            - Pausa Player 2
stop1             - Para Player 1
stop2             - Para Player 2
```

## 🔍 Comportamentos Esperados

### 🟢 Estado Ready
- **Player 1**: LED 15 verde
- **Player 2**: LED 7 verde
- **Outros LEDs**: Apagados

### 🔴 Player 1 Ativo
- **Progressão**: 15→14→13→12→11→10→9→8 (vermelho)
- **Ready**: LED 15 verde
- **Duração**: 5 segundos

### 🔵 Player 2 Ativo
- **Progressão**: 7→6→5→4→3→2→1→0 (azul)
- **Ready**: LED 7 verde
- **Duração**: 5 segundos

### 🟠 Efeito Laranja (VR Problema)
- **Quando**: `vr_hibernate`, `vr_signal_lost`, `vr_disconnected`
- **Efeito**: Todos os LEDs piscam laranja a cada 500ms
- **Ação**: Para todas as sessões ativas

## 📊 Status Messages (Recebidos)

### Conexão
```
ready1            - Player 1 pronto
ready2            - Player 2 pronto
```

### Progresso Player 1
```
status1:0         - Início
status1:20        - 20% progresso
status1:50        - 50% progresso
status1:100       - Completo
status1:paused    - Pausado
status1:stopped   - Parado
status1:complete  - Sessão completa
```

### Progresso Player 2
```
status2:0         - Início
status2:20        - 20% progresso
status2:50        - 50% progresso
status2:100       - Completo
status2:paused    - Pausado
status2:stopped   - Parado
status2:complete  - Sessão completa
```

## 🚨 Troubleshooting

### Se LEDs não respondem:
1. Verificar conexão WebSocket
2. Enviar `vr_connected` para resetar
3. Verificar se ESP32 está no hotspot "CoralVivoVR"

### Se efeito laranja não para:
1. Enviar `vr_connected` para sair do modo problema
2. Verificar se não há comandos de VR ativos

### Se sessões não iniciam:
1. Verificar se VR está conectado (`vr_connected`)
2. Verificar se não há efeito laranja ativo
3. Enviar `clear` para limpar LEDs
