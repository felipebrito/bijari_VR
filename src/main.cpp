#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <FastLED.h>

// Configuração
#define LED_PIN 2
#define BUTTON1 4
#define BUTTON2 5
#define NUM_LEDS 16

CRGB leds[NUM_LEDS];
WebSocketsServer webSocket(80);

// Estados dos botões
bool lastButton1 = HIGH;
bool lastButton2 = HIGH;
unsigned long lastTime1 = 0;
unsigned long lastTime2 = 0;
unsigned long buttonPressTime1 = 0;
unsigned long buttonPressTime2 = 0;
unsigned long lastButtonClick1 = 0;
unsigned long lastButtonClick2 = 0;
bool stopTriggered1 = false;
bool stopTriggered2 = false;

// Estados das sessões
bool session1Active = false;
bool session2Active = false;
bool session1Paused = false;
bool session2Paused = false;
unsigned long session1Start = 0;
unsigned long session2Start = 0;
int session1Progress = 0;
int session2Progress = 0;

// Estados de conexão VR
bool vrConnected = false;
bool vrHibernated = false;
bool vrSignalLost = false;
unsigned long lastVRActivity = 0;
unsigned long vrTimeout = 10000; // 10 segundos sem atividade = perda de sinal
unsigned long lastOrangeBlink = 0;
bool orangeBlinkState = false;

// Estados de conexão VR por player
bool vrConnected1 = false;
bool vrHibernated1 = false;
bool vrSignalLost1 = false;
bool vrConnected2 = false;
bool vrHibernated2 = false;
bool vrSignalLost2 = false;

// Variáveis de efeito laranja por player
unsigned long lastOrangeBlink1 = 0;
bool orangeBlinkState1 = false;
unsigned long lastOrangeBlink2 = 0;
bool orangeBlinkState2 = false;

// Declarações das funções
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
void blink1();
void blink2();
void startSession1();
void startSession2();
void pauseSession1();
void pauseSession2();
void stopSession1();
void stopSession2();
// REMOVIDO: updateSession1 e updateSession2 - não simular mais
void setPlayer1Color(CRGB color, int brightness = 255);
void setPlayer2Color(CRGB color, int brightness = 255);
void setPlayer1Percent(int percent);
void setPlayer2Percent(int percent);
void clearAll();
void checkVRConnection();
void handleVRHibernate();
void handleVRSignalLost();
void orangeBlinkEffect();
void handleVRHibernate1();
void handleVRSignalLost1();
void handleVRConnected1();
void handleVRDisconnected1();
void handleVRHibernate2();
void handleVRSignalLost2();
void handleVRConnected2();
void handleVRDisconnected2();
void handleVideoEnded1();
void handleVideoEnded2();

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("=== CORALVIVO VR ===");
  
  // Configurar botões
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  
  // Configurar LEDs
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  
  // Limpar LEDs
  clearAll();
  
  // WiFi AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP("CoralVivoVR", "12345678");
  Serial.println("WiFi: CoralVivoVR");
  Serial.println("IP: " + WiFi.softAPIP().toString());
  
  // WebSocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket: 80");
  
  Serial.println("PRONTO!");
}

void loop() {
  webSocket.loop();
  
  // Botão 1
  bool reading1 = digitalRead(BUTTON1);
  if (reading1 == LOW && lastButton1 == HIGH) {
    // Botão pressionado - iniciar timer
    buttonPressTime1 = millis();
    stopTriggered1 = false;
    Serial.println("BOTAO 1 PRESSIONADO (iniciando timer)");
  }
  if (reading1 == LOW && lastButton1 == LOW) {
    // Botão ainda pressionado - verificar se segurou por tempo suficiente
    unsigned long pressDuration = millis() - buttonPressTime1;
    if (pressDuration > 2000 && !stopTriggered1) {
      // Segurou por mais de 2 segundos = LONG PRESS (apenas uma vez)
      Serial.println("BOTAO 1 LONG PRESS!");
      webSocket.broadcastTXT("long1");
      stopTriggered1 = true;
    }
  }
  if (reading1 == HIGH && lastButton1 == LOW) {
    // Botão solto - verificar se foi clique rápido
    unsigned long pressDuration = millis() - buttonPressTime1;
    unsigned long currentTime = millis();
    
    if (pressDuration < 2000 && pressDuration > 50 && !stopTriggered1) {
      // Debounce: evitar cliques muito rápidos (menos de 300ms entre cliques)
      if (currentTime - lastButtonClick1 > 300) {
        // Clique rápido = BOTÃO PRESSIONADO
        Serial.println("BOTAO 1 CLIQUE RAPIDO!");
        // Enviar apenas sinal de botão - Unity decide se é play ou pause
        webSocket.broadcastTXT("button1");
        lastButtonClick1 = currentTime;
      } else {
        Serial.println("BOTAO 1 CLIQUE IGNORADO (debounce)");
      }
    }
  }
  lastButton1 = reading1;
  
  // Botão 2
  bool reading2 = digitalRead(BUTTON2);
  if (reading2 == LOW && lastButton2 == HIGH) {
    // Botão pressionado - iniciar timer
    buttonPressTime2 = millis();
    stopTriggered2 = false;
    Serial.println("BOTAO 2 PRESSIONADO (iniciando timer)");
  }
  if (reading2 == LOW && lastButton2 == LOW) {
    // Botão ainda pressionado - verificar se segurou por tempo suficiente
    unsigned long pressDuration = millis() - buttonPressTime2;
    if (pressDuration > 2000 && !stopTriggered2) {
      // Segurou por mais de 2 segundos = LONG PRESS (apenas uma vez)
      Serial.println("BOTAO 2 LONG PRESS!");
      webSocket.broadcastTXT("long2");
      stopTriggered2 = true;
    }
  }
  if (reading2 == HIGH && lastButton2 == LOW) {
    // Botão solto - verificar se foi clique rápido
    unsigned long pressDuration = millis() - buttonPressTime2;
    unsigned long currentTime = millis();
    
    if (pressDuration < 2000 && pressDuration > 50 && !stopTriggered2) {
      // Debounce: evitar cliques muito rápidos (menos de 300ms entre cliques)
      if (currentTime - lastButtonClick2 > 300) {
        // Clique rápido = BOTÃO PRESSIONADO
        Serial.println("BOTAO 2 CLIQUE RAPIDO!");
        // Enviar apenas sinal de botão - Unity decide se é play ou pause
        webSocket.broadcastTXT("button2");
        lastButtonClick2 = currentTime;
      } else {
        Serial.println("BOTAO 2 CLIQUE IGNORADO (debounce)");
      }
    }
  }
  lastButton2 = reading2;
  
  // REMOVIDO: simulação interna de sessões
  // Agora os percentuais vêm do Unity via percent1:X e percent2:X
  
  // Verificar conexão VR
  checkVRConnection();
  
  // Efeito laranja GLOBAL REMOVIDO - usar apenas individualizado por player
  // if (vrHibernated || vrSignalLost) {
  //   orangeBlinkEffect();
  // }
  
  // Efeito laranja por player
  if (vrHibernated1 || vrSignalLost1) {
    // Piscar LEDs do Player 1 em laranja
    unsigned long currentTime = millis();
    if (currentTime - lastOrangeBlink1 > 500) {
      orangeBlinkState1 = !orangeBlinkState1;
      lastOrangeBlink1 = currentTime;
      
      if (orangeBlinkState1) {
        for (int i = 8; i < 16; i++) leds[i] = CRGB::Orange;
          } else {
        for (int i = 8; i < 16; i++) leds[i] = CRGB::Black;
      }
      FastLED.show();
    }
  }
  
  if (vrHibernated2 || vrSignalLost2) {
    // Piscar LEDs do Player 2 em laranja
    unsigned long currentTime = millis();
    if (currentTime - lastOrangeBlink2 > 500) {
      orangeBlinkState2 = !orangeBlinkState2;
      lastOrangeBlink2 = currentTime;
      
      if (orangeBlinkState2) {
        for (int i = 0; i < 8; i++) leds[i] = CRGB::Orange;
    } else {
        for (int i = 0; i < 8; i++) leds[i] = CRGB::Black;
      }
      FastLED.show();
    }
  }
  
  delay(10);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_CONNECTED) {
    Serial.println("CLIENTE CONECTADO!");
    // Variáveis globais REMOVIDAS - usar apenas individualizadas
    // vrConnected = true;
    // vrHibernated = false;
    // vrSignalLost = false;
    // lastVRActivity = millis();
    
    // NÃO enviar ready automaticamente - cada player envia vr_connectedX primeiro
    // webSocket.broadcastTXT("ready1");
    // webSocket.broadcastTXT("ready2");
    
    // NÃO acender LEDs automaticamente - cada player controla quando conecta
    // leds[15] = CRGB::Green; // Player 1 ready (LED 15)
    // leds[7] = CRGB::Green;  // Player 2 ready (último LED da timeline azul)
    // FastLED.show();
  }
  else if (type == WStype_DISCONNECTED) {
    Serial.println("CLIENTE DESCONECTADO!");
    
    // Como não sabemos qual player desconectou, ativar efeito laranja para ambos
    // O Unity vai reconectar e enviar vr_connectedX para identificar qual player
    if (vrConnected1) {
      vrSignalLost1 = true;
      vrConnected1 = false;
      Serial.println("*** PLAYER 1 - PERDA DE SINAL (desconexão) ***");
    }
    if (vrConnected2) {
      vrSignalLost2 = true;
      vrConnected2 = false;
      Serial.println("*** PLAYER 2 - PERDA DE SINAL (desconexão) ***");
    }
  }
  else if (type == WStype_TEXT) {
    String msg = String((char*)payload);
    // Remover caracteres de controle (CR, LF, espaços)
    msg.trim();
    Serial.println("*** WEBSOCKET MSG: " + msg + " ***");
    Serial.println("*** MSG LENGTH: " + String(msg.length()) + " ***");
    
    // Ignorar comandos que o próprio ESP32 enviou
    if (msg == "ready1" || msg == "ready2" || 
        msg.startsWith("status1:") || msg.startsWith("status2:")) {
      Serial.println("*** COMANDO INTERNO IGNORADO ***");
    return;
  }
    
    // Atualizar atividade VR REMOVIDO - usar apenas individualizado
    // if (vrConnected) {
    //   lastVRActivity = millis();
    // }
    
    // REMOVIDO: processamento de play1/pause1 - Unity não envia mais
    // Agora Unity envia apenas button1/button2 e long1/long2
    if (msg == "pause1") {
      pauseSession1();
    }
    else if (msg == "stop1") {
      stopSession1();
    }
    else if (msg == "pause2") {
      pauseSession2();
    }
    else if (msg == "stop2") {
      stopSession2();
    }
    else if (msg == "red1") {
      setPlayer1Color(CRGB::Red);
    }
    else if (msg == "orange1") {
      setPlayer1Color(CRGB::Orange);
    }
    else if (msg == "yellow1") {
      setPlayer1Color(CRGB::Yellow);
    }
    else if (msg == "green1") {
      setPlayer1Color(CRGB::Green);
    }
    else if (msg == "blue1") {
      setPlayer1Color(CRGB::Blue);
    }
    else if (msg == "purple1") {
      setPlayer1Color(CRGB::Purple);
    }
    else if (msg == "red2") {
      setPlayer2Color(CRGB::Red);
    }
    else if (msg == "orange2") {
      setPlayer2Color(CRGB::Orange);
    }
    else if (msg == "yellow2") {
      setPlayer2Color(CRGB::Yellow);
    }
    else if (msg == "green2") {
      setPlayer2Color(CRGB::Green);
    }
    else if (msg == "blue2") {
      setPlayer2Color(CRGB::Blue);
    }
    else if (msg == "purple2") {
      setPlayer2Color(CRGB::Purple);
    }
    else if (msg == "clear1") {
      for (int i = 8; i < 16; i++) leds[i] = CRGB::Black;
      FastLED.show();
    }
    else if (msg == "clear2") {
      for (int i = 0; i < 8; i++) leds[i] = CRGB::Black;
      FastLED.show();
    }
    // Comandos de percentual por player
    else if (msg.startsWith("percent1:")) {
      int percent = msg.substring(9).toInt();
      if (percent >= 0 && percent <= 100) {
        Serial.println("*** PLAYER 1 PERCENTUAL: " + String(percent) + "% ***");
        setPlayer1Percent(percent);
      }
    }
    else if (msg.startsWith("percent2:")) {
      int percent = msg.substring(9).toInt();
      if (percent >= 0 && percent <= 100) {
        Serial.println("*** PLAYER 2 PERCENTUAL: " + String(percent) + "% ***");
        setPlayer2Percent(percent);
      }
    }
    // Comandos de VR genéricos REMOVIDOS - usar apenas individualizados
    // else if (msg == "vr_hibernate") {
    //   handleVRHibernate();
    // }
    // else if (msg == "vr_signal_lost") {
    //   handleVRSignalLost();
    // }
    // Comando genérico vr_connected REMOVIDO - usar apenas individualizados
    // else if (msg == "vr_connected") {
    //   vrConnected = true;
    //   vrHibernated = false;
    //   vrSignalLost = false;
    //   lastVRActivity = millis();
    //   Serial.println("*** VR CONECTADO ***");
    //   // Voltar ao estado ready
    //   leds[15] = CRGB::Green;
    //   leds[7] = CRGB::Green;
    //   for (int i = 0; i < 7; i++) leds[i] = CRGB::Black;
    //   for (int i = 8; i < 15; i++) leds[i] = CRGB::Black;
    //   FastLED.show();
    // }
    // Comando genérico vr_disconnected REMOVIDO - usar apenas individualizados
    // else if (msg == "vr_disconnected") {
    //   vrConnected = false;
    //   vrHibernated = false;
    //   vrSignalLost = true;
    //   lastVRActivity = 0;
    //   Serial.println("*** VR DESCONECTADO ***");
    // }
    // Comandos de VR por player
    else if (msg == "vr_hibernate1") {
      Serial.println("*** PROCESSANDO VR_HIBERNATE1 ***");
      handleVRHibernate1();
    }
    else if (msg == "vr_signal_lost1") {
      Serial.println("*** PROCESSANDO VR_SIGNAL_LOST1 ***");
      handleVRSignalLost1();
    }
    else if (msg == "vr_connected1") {
      Serial.println("*** PROCESSANDO VR_CONNECTED1 ***");
      handleVRConnected1();
    }
    else if (msg == "vr_disconnected1") {
      Serial.println("*** PROCESSANDO VR_DISCONNECTED1 ***");
      handleVRDisconnected1();
    }
    else if (msg == "vr_hibernate2") {
      Serial.println("*** PROCESSANDO VR_HIBERNATE2 ***");
      handleVRHibernate2();
    }
    else if (msg == "vr_signal_lost2") {
      Serial.println("*** PROCESSANDO VR_SIGNAL_LOST2 ***");
      handleVRSignalLost2();
    }
    else if (msg == "vr_connected2") {
      Serial.println("*** PROCESSANDO VR_CONNECTED2 ***");
      handleVRConnected2();
    }
    else if (msg == "vr_disconnected2") {
      Serial.println("*** PROCESSANDO VR_DISCONNECTED2 ***");
      handleVRDisconnected2();
    }
    else if (msg == "video_ended1") {
      Serial.println("*** PROCESSANDO VIDEO_ENDED1 ***");
      handleVideoEnded1();
    }
    else if (msg == "video_ended2") {
      Serial.println("*** PROCESSANDO VIDEO_ENDED2 ***");
      handleVideoEnded2();
    }
    else {
      Serial.println("*** COMANDO DESCONHECIDO: " + msg + " ***");
    }
  }
}

void startSession1() {
  session1Active = true;
  session1Paused = false;
  session1Start = millis();
  session1Progress = 0;
  Serial.println("*** SESSÃO 1 INICIADA ***");
  webSocket.broadcastTXT("status1:0");
}

void startSession2() {
  session2Active = true;
  session2Paused = false;
  session2Start = millis();
  session2Progress = 0;
  Serial.println("*** SESSÃO 2 INICIADA ***");
  webSocket.broadcastTXT("status2:0");
}

void pauseSession1() {
  session1Paused = true;
  Serial.println("*** SESSÃO 1 PAUSADA ***");
  webSocket.broadcastTXT("status1:paused");
}

void pauseSession2() {
  session2Paused = true;
  Serial.println("*** SESSÃO 2 PAUSADA ***");
  webSocket.broadcastTXT("status2:paused");
}

void stopSession1() {
  session1Active = false;
  session1Paused = false;
  session1Progress = 0;
  Serial.println("*** SESSÃO 1 PARADA ***");
  webSocket.broadcastTXT("status1:stopped");
  
  // Limpar TODA a timeline vermelha (LEDs 8-15) e voltar ao estado ready
  for (int i = 8; i < 16; i++) {
    leds[i] = CRGB::Black;
  }
  leds[15] = CRGB::Green; // Apenas o LED ready verde
  FastLED.show();
}

void stopSession2() {
  session2Active = false;
  session2Paused = false;
  session2Progress = 0;
  Serial.println("*** SESSÃO 2 PARADA ***");
  webSocket.broadcastTXT("status2:stopped");
  
  // Limpar TODA a timeline azul (LEDs 0-7) e voltar ao estado ready
  for (int i = 0; i < 8; i++) {
    leds[i] = CRGB::Black;
  }
  leds[7] = CRGB::Green; // Apenas o LED ready verde
  FastLED.show();
}

// REMOVIDO: updateSession1() - não simular mais
// Agora os percentuais vêm do Unity via percent1:X

// REMOVIDO: updateSession2() - não simular mais
// Agora os percentuais vêm do Unity via percent2:X

void setPlayer1Color(CRGB color, int brightness) {
  for (int i = 8; i < 16; i++) {
    leds[i] = color;
    leds[i].nscale8(brightness);
  }
  FastLED.show();
}

void setPlayer2Color(CRGB color, int brightness) {
  for (int i = 0; i < 8; i++) {
    leds[i] = color;
    leds[i].nscale8(brightness);
  }
  FastLED.show();
}

void clearAll() {
  for (int i = 0; i < 16; i++) {
      leds[i] = CRGB::Black;
  }
  FastLED.show();
}

// Funções de percentual por player
void setPlayer1Percent(int percent) {
  // Player 1: LEDs 8-15, sequência inversa (15→14→13...→8)
  // Cada LED representa 12.5% (100% / 8 LEDs)
  
  // Primeiro, apagar todos os LEDs do Player 1
  for (int i = 8; i < 16; i++) {
    leds[i] = CRGB::Black;
  }
  
  // Calcular quantos LEDs completos devem estar acesos (12.5% cada)
  int fullLEDs = (percent * 8) / 100;  // percent * 8 / 100 = quantos LEDs completos
  int partialLED = (percent * 8) % 100;  // resto para LED parcial
  
  // Acender LEDs completos (sequência inversa: 15→14→13...)
  for (int i = 0; i < fullLEDs && i < 8; i++) {
    leds[15 - i] = CRGB::Red;
  }
  
  // Acender LED parcial (se houver)
  if (partialLED > 0 && fullLEDs < 8) {
    int ledIndex = 15 - fullLEDs;
    int intensity = map(partialLED, 0, 100, 51, 255); // 51 = 20% de 255
    leds[ledIndex] = CRGB::Red;
    leds[ledIndex].nscale8(intensity);
  }
  
  // Se 100%, manter LED ready (15) verde
  if (percent >= 100) {
    leds[15] = CRGB::Green;
  }
  
  FastLED.show();
}

void setPlayer2Percent(int percent) {
  // Player 2: LEDs 0-7, sequência normal (0→1→2...→7)
  // Cada LED representa 12.5% (100% / 8 LEDs)
  
  // Primeiro, apagar todos os LEDs do Player 2
  for (int i = 0; i < 8; i++) {
          leds[i] = CRGB::Black;
  }
  
  // Calcular quantos LEDs completos devem estar acesos (12.5% cada)
  int fullLEDs = (percent * 8) / 100;  // percent * 8 / 100 = quantos LEDs completos
  int partialLED = (percent * 8) % 100;  // resto para LED parcial
  
  // Acender LEDs completos (sequência normal: 0→1→2...)
  for (int i = 0; i < fullLEDs && i < 8; i++) {
        leds[i] = CRGB::Blue;
      }
      
  // Acender LED parcial (se houver)
  if (partialLED > 0 && fullLEDs < 8) {
    int ledIndex = fullLEDs;
    int intensity = map(partialLED, 0, 100, 51, 255); // 51 = 20% de 255
    leds[ledIndex] = CRGB::Blue;
    leds[ledIndex].nscale8(intensity);
  }
  
  // Se 100%, manter LED ready (7) verde
  if (percent >= 100) {
    leds[7] = CRGB::Green;
  }
  
  FastLED.show();
}

void blink1() {
  for (int i = 0; i < 3; i++) {
    for (int j = 8; j < 16; j++) leds[j] = CRGB::Red;
    FastLED.show();
    delay(200);
    for (int j = 8; j < 16; j++) leds[j] = CRGB::Black;
    FastLED.show();
    delay(200);
  }
}

void blink2() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 8; j++) leds[j] = CRGB::Blue;
    FastLED.show();
    delay(200);
    for (int j = 0; j < 8; j++) leds[j] = CRGB::Black;
    FastLED.show();
    delay(200);
  }
}

// Funções de VR
void checkVRConnection() {
  // Timeout desabilitado - só ativa via comando manual
  // if (vrConnected && lastVRActivity > 0) {
  //   unsigned long timeSinceActivity = millis() - lastVRActivity;
  //   if (timeSinceActivity > vrTimeout) {
  //     // Timeout - perda de sinal
  //     vrSignalLost = true;
  //     vrConnected = false;
  //     Serial.println("*** VR TIMEOUT - PERDA DE SINAL ***");
  //   }
  // }
}

void handleVRHibernate() {
  vrHibernated = true;
  vrConnected = false;
  vrSignalLost = false;
  Serial.println("*** VR HIBERNADO ***");
  // Parar todas as sessões
  if (session1Active) {
    session1Active = false;
    session1Paused = false;
    session1Progress = 0;
    webSocket.broadcastTXT("status1:stopped");
  }
  if (session2Active) {
    session2Active = false;
    session2Paused = false;
    session2Progress = 0;
    webSocket.broadcastTXT("status2:stopped");
  }
}

void handleVRSignalLost() {
  vrSignalLost = true;
  vrConnected = false;
  vrHibernated = false;
  Serial.println("*** VR PERDA DE SINAL ***");
  // Parar todas as sessões
  if (session1Active) {
    session1Active = false;
    session1Paused = false;
    session1Progress = 0;
    webSocket.broadcastTXT("status1:stopped");
  }
  if (session2Active) {
    session2Active = false;
    session2Paused = false;
    session2Progress = 0;
    webSocket.broadcastTXT("status2:stopped");
  }
}

void orangeBlinkEffect() {
  unsigned long currentTime = millis();
  if (currentTime - lastOrangeBlink > 500) { // Piscar a cada 500ms
    orangeBlinkState = !orangeBlinkState;
    lastOrangeBlink = currentTime;
    
    if (orangeBlinkState) {
      // Acender todos os LEDs em laranja
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Orange;
      }
    } else {
      // Apagar todos os LEDs
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
    }
    FastLED.show();
  }
}

// Funções de VR por player
void handleVRHibernate1() {
  vrHibernated1 = true;
  vrConnected1 = false;
  vrSignalLost1 = false;
  Serial.println("*** VR PLAYER 1 HIBERNADO ***");
  // Parar sessão ativa
  if (session1Active) {
    stopSession1();
  }
}

void handleVRSignalLost1() {
  vrSignalLost1 = true;
  vrConnected1 = false;
  vrHibernated1 = false;
  Serial.println("*** VR PLAYER 1 - PERDA DE SINAL ***");
  // Parar sessão ativa
  if (session1Active) {
    stopSession1();
  }
}

void handleVRConnected1() {
  vrConnected1 = true;
  vrHibernated1 = false;
  vrSignalLost1 = false;
  Serial.println("*** VR PLAYER 1 CONECTADO ***");
  
  // Enviar ready apenas para Player 1
  webSocket.broadcastTXT("ready1");
  
  // Voltar ao estado ready
  leds[15] = CRGB::Green;
  for (int i = 8; i < 15; i++) leds[i] = CRGB::Black;
  FastLED.show();
}

void handleVRDisconnected1() {
  vrConnected1 = false;
  vrHibernated1 = false;
  vrSignalLost1 = true;
  Serial.println("*** VR PLAYER 1 DESCONECTADO ***");
  // Parar sessão ativa
  if (session1Active) {
    stopSession1();
  }
}

void handleVRHibernate2() {
  vrHibernated2 = true;
  vrConnected2 = false;
  vrSignalLost2 = false;
  Serial.println("*** VR PLAYER 2 HIBERNADO ***");
  // Parar sessão ativa
  if (session2Active) {
    stopSession2();
  }
}

void handleVRSignalLost2() {
  vrSignalLost2 = true;
  vrConnected2 = false;
  vrHibernated2 = false;
  Serial.println("*** VR PLAYER 2 - PERDA DE SINAL ***");
  // Parar sessão ativa
  if (session2Active) {
    stopSession2();
  }
}

void handleVRConnected2() {
  vrConnected2 = true;
  vrHibernated2 = false;
  vrSignalLost2 = false;
  Serial.println("*** VR PLAYER 2 CONECTADO ***");
  
  // Enviar ready apenas para Player 2
  webSocket.broadcastTXT("ready2");
  
  // Voltar ao estado ready (último LED da timeline azul)
  leds[7] = CRGB::Green;
  for (int i = 0; i < 7; i++) leds[i] = CRGB::Black;
  FastLED.show();
}

void handleVRDisconnected2() {
  vrConnected2 = false;
  vrHibernated2 = false;
  vrSignalLost2 = true;
  Serial.println("*** VR PLAYER 2 DESCONECTADO ***");
  // Parar sessão ativa
  if (session2Active) {
    stopSession2();
  }
}

void handleVideoEnded1() {
  Serial.println("*** VÍDEO PLAYER 1 TERMINADO ***");
  // Limpar timeline e voltar ao estado ready
  for (int i = 8; i < 16; i++) {
    leds[i] = CRGB::Black;
  }
  leds[15] = CRGB::Green; // Apenas o LED ready verde
  FastLED.show();
}

void handleVideoEnded2() {
  Serial.println("*** VÍDEO PLAYER 2 TERMINADO ***");
  // Limpar timeline e voltar ao estado ready
  for (int i = 0; i < 8; i++) {
    leds[i] = CRGB::Black;
  }
  leds[7] = CRGB::Green; // Apenas o LED ready verde
  FastLED.show();
}