# LED Test Simples - CoralVivoVR

Este é um código simplificado apenas para testar os LEDs do projeto CoralVivoVR.

## 🎯 Objetivo

Testar a funcionalidade básica dos LEDs sem toda a complexidade do sistema completo (WiFi, WebSocket, etc.).

## 🔧 Hardware Necessário

- **ESP32** (qualquer modelo)
- **16 LEDs WS2812B** (fita LED)
- **Resistor 330Ω** (opcional, para proteção)
- **Capacitor 1000µF** (opcional, para estabilização de energia)

## 📌 Conexões

```
ESP32 GPIO 2 → LED Data (DIN)
ESP32 3.3V → LED VCC
ESP32 GND → LED GND
```

## 🚀 Como Usar

### Opção 1: PlatformIO (Recomendado)

1. **Instale o PlatformIO**:
   ```bash
   pip install platformio
   ```

2. **Compile e faça upload**:
   ```bash
   # Renomeie o arquivo de configuração
   mv platformio_test.ini platformio.ini
   
   # Compile
   pio run
   
   # Faça upload para o ESP32
   pio run --target upload
   
   # Monitore a saída serial
   pio device monitor
   ```

### Opção 2: Arduino IDE

1. **Instale a biblioteca FastLED**:
   - Tools → Manage Libraries
   - Procure por "FastLED"
   - Instale a versão mais recente

2. **Configure o ESP32**:
   - Tools → Board → ESP32 Dev Module
   - Tools → Port → (selecione a porta do ESP32)

3. **Compile e faça upload**

## 🎨 Testes Realizados

O código executa os seguintes testes em sequência:

| Teste | Descrição | Duração |
|-------|-----------|---------|
| **1** | Todos os LEDs azuis | 2s |
| **2** | Todos os LEDs vermelhos | 2s |
| **3** | Todos os LEDs verdes | 2s |
| **4** | Player 1 (azul) + Player 2 (vermelho) | 2s |
| **5** | Progressão Player 1 (azul) | 2s |
| **6** | Progressão Player 2 (vermelho) | 2s |
| **7** | Efeito Rainbow | 3s |
| **8** | Efeito Chase (perseguição) | 3s |
| **9** | Piscar todos os LEDs | 2s |

## 📊 Saída Serial

```
LED Test - Iniciando...
LEDs inicializados! Testando...
Teste 1: Todos os LEDs azuis
Teste 2: Todos os LEDs vermelhos
Teste 3: Todos os LEDs verdes
Teste 4: Player 1 (azul) e Player 2 (vermelho)
Teste 5: Progressão Player 1
Teste 6: Progressão Player 2
Teste 7: Efeito Rainbow
Teste 8: Efeito Chase
Teste 9: Piscar todos os LEDs
Limpando LEDs...
```

## ⚙️ Configurações

### LEDs
- **Pino**: GPIO 2
- **Quantidade**: 16 LEDs
- **Player 1**: LEDs 0-7 (esquerda)
- **Player 2**: LEDs 8-15 (direita)
- **Brilho**: 50 (ajustável)

### Cores Padrão
- **Player 1**: Azul (`CRGB::Blue`)
- **Player 2**: Vermelho (`CRGB::Red`)
- **Ready**: Verde (`CRGB::Green`)
- **Offline**: Roxo (`CRGB(128, 0, 128)`)

## 🔧 Personalização

### Alterar Brilho
```cpp
FastLED.setBrightness(100); // 0-255
```

### Alterar Cores
```cpp
// Player 1 - Verde
leds[i] = CRGB::Green;

// Player 2 - Amarelo
leds[i] = CRGB::Yellow;

// Cor personalizada (R, G, B)
leds[i] = CRGB(255, 128, 0); // Laranja
```

### Alterar Velocidade dos Testes
```cpp
delay(1000); // 1 segundo entre testes
```

## 🐛 Solução de Problemas

### LEDs não acendem
1. Verifique as conexões
2. Confirme se o GPIO 2 está correto
3. Teste com brilho máximo: `FastLED.setBrightness(255)`

### LEDs com cores erradas
1. Verifique a ordem GRB no código: `GRB>(leds, NUM_LEDS)`
2. Alguns LEDs podem usar RGB ao invés de GRB

### ESP32 não conecta
1. Verifique se o cabo USB está funcionando
2. Confirme se os drivers estão instalados
3. Teste com outro cabo USB

## 📝 Próximos Passos

Após confirmar que os LEDs estão funcionando:

1. **Volte ao código completo** (`main.cpp`)
2. **Teste a conectividade WiFi**
3. **Teste os comandos WebSocket**
4. **Integre com Unity**

## 📚 Referências

- [FastLED Library](https://github.com/FastLED/FastLED)
- [ESP32 Pinout](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-devkit-c.html)
- [WS2812B Datasheet](https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf)
