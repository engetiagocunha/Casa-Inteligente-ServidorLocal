#ifndef SENSORS_HPP
#define SENSORS_HPP

#include <DHT.h>  // Inclui a biblioteca DHT
#include "SetupGPIOs.hpp"

// Definições das variáveis globais
float temperatura;
float umidade;

DHT dht(DHTPIN, DHTTYPE);

// Funções para inicializar e ler o sensor DHT
void setupSensors() {
  dht.begin();  // Inicializa o sensor DHT com o pino e o tipo definidos
}

void readDHT() {
  umidade = dht.readHumidity();         // Lê a umidade
  temperatura = dht.readTemperature();  // Lê a temperatura em Celsius

  // Verifica se as leituras são válidas
  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Falha ao ler do sensor DHT!");
  }

  // Exibe os dados dos sensores
  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.println(" %");

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");
}

#endif  // SENSORS_HPP
