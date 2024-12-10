#include <WiFi.h>
#include <PubSubClient.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define LIGHT_SENSOR_PIN 35
#define SEALEVELPRESSURE_HPA (1013.25)

// Criar objeto para o BME280
Adafruit_BME280 bme; 


// WiFi
const char *ssid = "Caio"; // Enter your Wi-Fi name
const char *password = "caio1234";  // Enter Wi-Fi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "emqx/esp32docaionoshare"; // tropicalia/sensor/umidade/valor
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

unsigned long lastMsgTime = 0;  // Variável para armazenar o tempo da última publicação
const long interval = 5000;     // Intervalo de 5 segundos
char ldrState[15];


WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    // Set software serial baud to 115200;
    Serial.begin(115200);
    // Connecting to a WiFi network
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the Wi-Fi network");
    //connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Public EMQX MQTT broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }

    if (!bme.begin(0x76)) {  
    Serial.println(F("Could not find a valid BME280 sensor, check wiring or try a different address!"));
    while (1) delay(10); // Loop infinito em caso de falha
  }

  Serial.println(F("BME280 detected!"));
    
    // Publish and subscribe
    client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

void loop() {
  client.loop();

  float temperature = bme.readTemperature();
  float pressure = bme.readPressure() / 100.0F;
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  float humidity = bme.readHumidity();

  int analogValue = analogRead(LIGHT_SENSOR_PIN);

  // Exibir as leituras no serial monitor
  Serial.print(F("Temperatura = "));
  Serial.print(temperature);
  Serial.println(" *C");

  Serial.print(F("Pressão = "));
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print(F("Approx altitude = "));
  Serial.print(altitude);
  Serial.println(" m");

  Serial.print(F("Umidade = "));
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print(F("Luz = "));
  Serial.print(analogValue);

  Serial.println();

  // Publicar as leituras no broker MQTT
  char payload[150];  // Buffer para a mensagem
  snprintf(payload, sizeof(payload), "{\"Temp\": %.2f, \"Press\": %.2f, \"Hum\": %.2f, \"Light\": \"%d\"}", temperature, pressure, humidity, analogValue);
  
  client.publish(topic, payload);
  Serial.println("Data published to MQTT broker");

  delay(5000); 
}
