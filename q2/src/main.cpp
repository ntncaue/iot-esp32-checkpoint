/////////--------IOT--------FIAP------------///////////

#include <Arduino.h>



// Configurações de Hardware

#define PIN_LED 15


void setup() {
  Serial.begin(115200);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

}

void loop() {
  digitalWrite(PIN_LED, HIGH);
  delay(400);
  digitalWrite(PIN_LED, LOW);
  delay(400);
  Serial.println("piscando...");
}









///// --------variação para publicar MQTT apenas se houver mudança do sensor

// // Variáveis globais para armazenar os últimos valores de temperatura e umidade
// float lastTemperature = NAN;
// float lastHumidity = NAN;
      // // Verificar se os novos valores de temperatura e umidade são diferentes dos últimos valores armazenados
      // if (sensorValues.temperature != lastTemperature || sensorValues.humidity != lastHumidity) {

        // // Atualizar os valores armazenados com os novos valores
        // lastTemperature = sensorValues.temperature;
        // lastHumidity = sensorValues.humidity;