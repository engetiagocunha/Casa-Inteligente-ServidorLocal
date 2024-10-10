#ifndef WIFI_SETUP_HPP
#define WIFI_SETUP_HPP

#include <ESPmDNS.h>      // Biblioteca para mDNS
#include <WiFiManager.h>  // Biblioteca para WiFiManager
#include "SetupGPIOs.hpp"

WiFiManager wm;  // Instância global do WiFiManager

// Função para configurar o WiFiManager
void setupWiFi() {
  WiFi.mode(WIFI_STA);  // Define o modo Wi-Fi para estação (STA)
  Serial.println("\nIniciando configuração do WiFi...");

  // Configuração WiFiManager
  bool wm_nonblocking = false;  // Define o modo não bloqueante
  if (wm_nonblocking) wm.setConfigPortalBlocking(false);

  // Menu customizado
  std::vector<const char*> menu = { "wifi", "info", "exit" };
  wm.setMenu(menu);
  wm.setClass("invert");          // Define o tema invertido
  wm.setConfigPortalTimeout(30);  // Tempo limite de 30 segundos

  // Conectar automaticamente
  bool res = wm.autoConnect("ESP3201-CONFIG", "12345678");
  if (!res) {
    Serial.println("Falha na conexão ou tempo limite esgotado");
  } else {
    Serial.println("Conectado com sucesso!");
  }
}

// Função para Configuração do mDNS
void setupMDNS() {
  if (!MDNS.begin("esphome")) {
    Serial.println("Erro ao configurar o mDNS!");
    while (1) {
      delay(1000);  // Loop infinito se falhar
    }
  }
  Serial.println("mDNS iniciado. Acesse esphome.local");
  MDNS.addService("http", "tcp", 80);  // Serviço HTTP via mDNS
}

// Função para verificar se o botão de reset foi pressionado
void checkButtonReset() {
  if (digitalRead(RESET_BUTTON) == LOW) {
    Serial.println("Botão pressionado ...");
    delay(50);
    if (digitalRead(RESET_BUTTON) == LOW) {
      Serial.println("Botão pressionado, mantenha por 3s");
      delay(3000);  // Verifica se o botão continua pressionado
      if (digitalRead(RESET_BUTTON) == LOW) {
        Serial.println("Apagando as configurações do WiFi e reiniciando...");
        wm.resetSettings();  // Limpa as configurações do WiFi
        ESP.restart();       // Reinicia o ESP32
      }
    }
  }
}

#endif  // WIFI_SETUP_HPP