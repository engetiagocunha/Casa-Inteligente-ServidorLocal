#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <Preferences.h>
#include <DHT.h>
#include <Ticker.h>
#include "src/SetupGPIOs.hpp"
#include "src/SetupWiFi.hpp"
#include "src/Sensors.hpp"
#include "src/PageHTML.hpp"

// Definições de variáveis
Ticker sensorTicker;
WebServer server(80);
WebSocketsServer webSocket(81);
Preferences preferences;
bool loggedIn = false;
const char* defaultUsername = "admin";
const char* defaultPassword = "admin";

// Função de configuração
void setup() {
  Serial.begin(115200);
  setupGPIOs();    // Configura GPIOs
  setupSensors();  // Inicializa o sensor DHT
  setupWiFi();     // Configura a conexão Wi-Fi
  setupMDNS();     // Configura a conexão mDNS
  setupServer();   // Configura o servidor web e WebSocket

  // Recupera estados salvos dos dispositivos
  preferences.begin("deviceStates", false);
  for (int i = 0; i < numDevices; i++) {
    deviceStates[i] = preferences.getBool(String("device" + String(i)).c_str(), false);
    digitalWrite(devicePins[i], deviceStates[i] ? LOW : HIGH);  // Aplica o estado salvo
  }

  calculateTouchMedians();                   // Calcula as medianas dos toques
  sensorTicker.attach(1, updateSensorData);  // Atualiza dados dos sensores a cada segundo
}

// Função principal de loop
void loop() {
  checkButtonReset();
  server.handleClient();
  webSocket.loop();
  handleTouchButtons();  // Lida com os botões touch capacitivos
}

// Função para atualizar os dados dos sensores
void updateSensorData() {
  readDHT();
  if (loggedIn) {
    notifyClients();
  }
}

// Configuração do servidor e WebSocket
void setupServer() {
  // Redireciona todas as rotas para a função handleRequests
  server.onNotFound(handleRequests);

  // Inicializa o servidor e o WebSocket
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

// Manipulação de requisições HTTP
void handleRequests() {
  String path = server.uri();

  // Página principal ("/")
  if (path == "/") {
    if (!loggedIn) {
      server.sendHeader("Location", "/login");
      server.send(303);
      return;
    }

    String html = String(HOME_PAGE);
    html.replace("%TEMPERATURE%", String(temperatura, 1));
    html.replace("%HUMIDITY%", String(umidade, 1));
    html.replace("%NUM_DEVICES%", String(numDevices));

    String buttons = "";
    for (int i = 0; i < numDevices; i++) {
      String btnClass = deviceStates[i] ? "on" : "off";
      buttons += "<div class=\"grid-item\"><button id=\"toggleBtn" + String(i) + "\" class=\"" + btnClass + "\" onclick=\"toggleDevice(" + String(i) + ")\">" + String(deviceStates[i] ? "Desligar" : "Ligar") + " Dispositivo " + String(i + 1) + "</button></div>";
    }
    html.replace("%BUTTONS%", buttons);

    server.send(200, "text/html", html);
  }

  // Página de login ("/login")
  else if (path == "/login") {
    if (server.hasArg("username") && server.hasArg("password")) {
      String username = server.arg("username");
      String password = server.arg("password");

      if (username == defaultUsername && password == defaultPassword) {
        loggedIn = true;
        server.sendHeader("Location", "/");
        server.send(303);
      } else {
        server.send(401, "text/plain", "Login falhou!");
      }
    } else {
      server.send(200, "text/html", LOGIN_PAGE);
    }
  }

  // Logout ("/logout")
  else if (path == "/logout") {
    loggedIn = false;
    server.sendHeader("Location", "/login");
    server.send(303);
  }

  // Página não encontrada (404)
  else {
    server.send(404, "text/plain", "404 Not Found");
  }
}

// Notifica os clientes via WebSocket
void notifyClients() {
  String message = "{\"deviceStates\":[";

  for (int i = 0; i < numDevices; i++) {
    message += deviceStates[i] ? "true" : "false";
    if (i < numDevices - 1) message += ",";
  }
  message += "],";
  message += "\"temperature\":" + String(temperatura) + ",";
  message += "\"humidity\":" + String(umidade) + "}";

  webSocket.broadcastTXT(message);
}

// Evento WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  if (type == WStype_TEXT) {
    int index = atoi((char*)payload);
    if (index >= 0 && index < numDevices) {
      deviceStates[index] = !deviceStates[index];
      digitalWrite(devicePins[index], deviceStates[index] ? LOW : HIGH);
      preferences.putBool(String("device" + String(index)).c_str(), deviceStates[index]);
      notifyClients();
    }
  }
}

// Função para botões touch capacitivos
void handleTouchButtons() {
  for (int i = 0; i < numTouchButtons; i++) {
    int touchValue = touchRead(touchButtonPins[i]);

    // Verifica se o toque foi registrado (abaixo do limiar)
    if (touchValue < touchMedians[i] - capacitanceThreshold && lastTouchStates[i] == HIGH) {
      // Alterna o estado do dispositivo correspondente
      deviceStates[i] = !deviceStates[i];
      digitalWrite(devicePins[i], deviceStates[i] ? LOW : HIGH);
      preferences.putBool(String("device" + String(i)).c_str(), deviceStates[i]);  // Salva o estado
      notifyClients();                                                             // Notifica os clientes conectados
    }

    // Atualiza o estado do toque
    lastTouchStates[i] = (touchValue < touchMedians[i] - capacitanceThreshold) ? LOW : HIGH;
  }
}

void calculateTouchMedians() {
  for (int i = 0; i < numTouchButtons; i++) {
    int sum = 0;
    for (int j = 0; j < 100; j++) {
      sum += touchRead(touchButtonPins[i]);
    }
    touchMedians[i] = sum / 100;  // Armazena a mediana calculada
  }
}
