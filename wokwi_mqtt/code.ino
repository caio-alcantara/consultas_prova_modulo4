#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
#define DHT22_PIN  21 

DHT dht22(DHT22_PIN, DHT22);

// WiFi
const char *ssid = "Wokwi-GUEST"; // Enter your Wi-Fi name
const char *password = "";  // Enter Wi-Fi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "emqx/esp32docaionoshare";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

unsigned long lastMsgTime = 0;  // Variável para armazenar o tempo da última publicação
const long interval = 5000;     // Intervalo de 5 segundos
WiFiClient espClient;
PubSubClient client(espClient);



void setup() {
  Serial.begin(9600);
  dht22.begin(); // initialize the DHT22 sensor

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the Wi-Fi network");

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

  // read humidity
  float humi  = dht22.readHumidity();
  // read temperature in Celsius
  float tempC = dht22.readTemperature();
  // read temperature in Fahrenheit
  float tempF = dht22.readTemperature(true);

  // check whether the reading is successful or not
  if ( isnan(tempC) || isnan(tempF) || isnan(humi)) {
    Serial.println("Failed to read from DHT22 sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  ");

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C  ~  ");
    Serial.print(tempF);
    Serial.println("°F");
  }

  // Publicar as leituras no broker MQTT
  char payload[150];  // Buffer para a mensagem
  snprintf(payload, sizeof(payload), "{\"Temp\": %.2f, \"Hum\": %.2f}", tempC, humi);
  
  client.publish(topic, payload);
  Serial.println("Data published to MQTT broker");

  delay(5000);
}
