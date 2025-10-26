# 🔧 Instruções para Upload Manual do ESP32

## ⚠️ Problema Atual
O ESP32 está travando durante o upload via PlatformIO. As correções estão prontas no código local.

## 🛠️ Soluções para Tentar

### 1. **Reset Manual do ESP32**
1. **Desconecte** o cabo USB
2. **Aguarde 10 segundos**
3. **Reconecte** o cabo USB
4. **Tente o upload novamente**

### 2. **Upload via Arduino IDE**
1. **Abra Arduino IDE**
2. **Instale ESP32 Board Package** (se não tiver)
3. **Selecione Board**: ESP32 Dev Module
4. **Selecione Port**: /dev/cu.usbserial-110
5. **Cole o código** de `src/main.cpp`
6. **Faça upload**

### 3. **Upload via esptool.py Direto**
```bash
# Navegar para o diretório do projeto
cd /Users/brito/Desktop/BIJARI_VR

# Fazer upload direto
esptool.py --chip esp32 --port /dev/cu.usbserial-110 --baud 19200 write_flash -z 0x1000 .pio/build/esp32dev/firmware.bin
```

### 4. **Verificar Hardware**
- **Cabo USB**: Tente outro cabo
- **Porta USB**: Tente outra porta USB
- **Botão RESET**: Pressione o botão RESET do ESP32 antes do upload

## 📋 Código Atualizado (Correções Implementadas)

### ✅ **Problemas Corrigidos:**

1. **Timeout Automático Desabilitado**
   - Timeout só ativa via comando manual (`vr_signal_lost`)
   - Não ativa automaticamente após 10 segundos

2. **Comandos Internos Ignorados**
   - `ready1`, `ready2`, `status1:X`, `status2:X` são ignorados
   - Evita "comando desconhecido" para mensagens internas

3. **Comandos VR Funcionais**
   - `vr_hibernate` - Pisca laranja
   - `vr_signal_lost` - Pisca laranja
   - `vr_connected` - Volta ao normal
   - `vr_disconnected` - Pisca laranja

## 🧪 Teste das Correções

### **Se conseguir fazer upload:**

1. **Conecte no WebSocket**: `ws://192.168.4.1:80`
2. **Teste comandos VR**:
   ```
   vr_hibernate      - Deve piscar laranja
   vr_signal_lost    - Deve piscar laranja
   vr_connected      - Deve voltar ao normal
   ```
3. **Teste sessões normais**:
   ```
   play1             - Player 1 (LEDs 8-15, vermelho)
   play2             - Player 2 (LEDs 0-7, azul)
   ```

### **Comportamento Esperado:**
- ✅ **Sem timeout automático**
- ✅ **Comandos internos ignorados**
- ✅ **Efeito laranja funcional**
- ✅ **Sessões normais funcionais**

## 🔄 Alternativa: Usar Código Anterior

Se não conseguir fazer upload, o código anterior (sem as correções) ainda funciona, mas com:
- ⚠️ Timeout automático ativando
- ⚠️ "Comando desconhecido" para ready1/ready2
- ✅ Funcionalidades básicas funcionando

## 📞 Próximos Passos

1. **Tente as soluções acima**
2. **Se funcionar**: Teste os comandos VR
3. **Se não funcionar**: Use o código anterior temporariamente
4. **Reporte o resultado** para próximos ajustes

