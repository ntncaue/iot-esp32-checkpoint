/********************************************************************
 * Projeto: Envio de Dados MQTT com ESP32
 * Autor: André Tritiack
 * Placa: DOIT ESP32 DEVKIT V1
 *
 * Descrição:
 * Este projeto conecta o ESP32 a uma rede Wi-Fi e a um Broker MQTT.
 * A cada 10 segundos, envia uma mensagem JSON com:
 * - ID do grupo
 * - ID do módulo
 * - IP local
 * - Dados de sensores simulados: Temperatura, Umidade, Pressão e Altitude
 *
 * Bibliotecas utilizadas:
 * - WiFi (nativa)
 * - PubSubClient (instalar)
 * - ArduinoJson (instalar)
 *
 * Baseado em tutoriais da Random Nerd Tutorials e documentação oficial
 * da Espressif.
 ********************************************************************/

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define boardLED 2 // LED onboard

// Identificadores
const char *ID = "fma-iot";
const char *moduleID = "ESP32_Q4";

// Wi-Fi
const char *SSID = "Wokwi-GUEST";
const char *PASSWORD = "";

// MQTT Broker
const char *BROKER_MQTT = "172.190.149.52";
const int BROKER_PORT = 1883;
const char *mqttUser = "admin_fiap";
const char *mqttPassword = "admin_fiap123";

// Tópico MQTT
#define TOPICO_PUBLISH "grupo/mottu/q4"

// Variáveis globais
WiFiClient espClient;
PubSubClient MQTT(espClient);
char buffer[256]; // Buffer para o JSON serializado

// Conexão Wi-Fi
void initWiFi()
{
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void reconectaWiFi()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Reconectando Wi-Fi...");
    initWiFi();
  }
}

// Conexão MQTT
void initMQTT()
{
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  while (!MQTT.connected())
  {
    Serial.println("Conectando ao Broker MQTT...");
    if (MQTT.connect(moduleID, mqttUser, mqttPassword))
    {
      Serial.println("Conectado ao Broker!");
    }
    else
    {
      Serial.print("Falha na conexão. Estado: ");
      Serial.println(MQTT.state());
      delay(2000);
    }
  }
}

void verificaConexoesWiFiEMQTT()
{
  reconectaWiFi();
  if (!MQTT.connected())
  {
    initMQTT();
  }
  MQTT.loop();
}

// Envio e feedback
void enviaEstadoOutputMQTT()
{
  MQTT.publish(TOPICO_PUBLISH, buffer);
  Serial.println("Mensagem publicada com sucesso!");
}

void piscaLed()
{
  digitalWrite(boardLED, HIGH);
  delay(300);
  digitalWrite(boardLED, LOW);
}

// Setup inicial
void setup()
{
  Serial.begin(115200);
  pinMode(boardLED, OUTPUT);
  digitalWrite(boardLED, LOW);
  initWiFi();
  initMQTT();
}

// Loop principal
void loop()
{
  // Verifica e mantém conexões ativas com Wi-Fi e MQTT
  verificaConexoesWiFiEMQTT();

  // Montagem do documento JSON
  StaticJsonDocument<300> doc;

  // 1. Identificação
  doc["ID"] = ID;
  doc["Sensor"] = moduleID;

  // 2. Rede
  doc["IP"] = WiFi.localIP().toString();

  // 3. Dados de sensores simulados
  float temperatura = random(200, 350) / 10.0; // 20.0°C a 35.0°C
  float umidade = random(400, 800) / 10.0;     // 40.0% a 80.0%
  float pressao = random(980, 1050);           // 980 hPa a 1050 hPa
  float altitude = random(0, 500);             // 0 m a 500 m

  doc["Temperatura"] = temperatura;
  doc["Umidade"] = umidade;
  doc["Pressao"] = pressao;
  doc["Altitude"] = altitude;

  // Serializa JSON para string
  serializeJson(doc, buffer);

  // Exibe no monitor serial
  Serial.println(buffer);

  // Envia via MQTT
  enviaEstadoOutputMQTT();

  // Feedback visual
  piscaLed();

  // Intervalo de envio
  delay(10000);
}
