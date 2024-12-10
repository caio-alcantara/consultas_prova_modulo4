#include "UbidotsEsp32Mqtt.h"

#define TRIG_PIN              19 
#define ECHO_PIN              18
#define LDR_RUA_PIN           35
#define LDR_CALCADA_PIN       32 
#define LED_GREEN_PIN_1       26
#define LED_YELLOW_PIN_1      25
#define LED_RED_PIN_1         33
#define LED_GREEN_PIN_2       23
#define LED_YELLOW_PIN_2      22
#define LED_RED_PIN_2         21

const char *UBIDOTS_TOKEN = "BBUS-0KKB8VTHx1jUYwZ3rUZJ659aEkq4bq";
const char *WIFI_SSID = "Inteli.Iot";
const char *WIFI_PASS = "@Intelix10T#";
const char *DEVICE_LABEL = "esp32_t11_g01";
const char *VARIABLE_LABEL_RED_1 = "luz_vermelha_1";
const char *VARIABLE_LABEL_YELLOW_1 = "luz_amarela_1";
const char *VARIABLE_LABEL_GREEN_1 = "luz_verde_1";
const char *VARIABLE_LABEL_LDR_RUA = "ldr_rua";
const char *VARIABLE_LABEL_LDR_CALCADA = "ldr_calcada";
const char *VARIABLE_LABEL_DISTANCE = "distance";
const int PUBLISH_FREQUENCY = 5000;

float duration_us, distance_cm;
int luz_calcada, luz_rua;
long int timer;

unsigned long greenStartTime = 0;
unsigned long yellowStartTime = 0;
bool isGreen = false;
bool isYellow = false;

Ubidots ubidots(UBIDOTS_TOKEN);

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  // Exibe a mensagem recebida
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  Serial.println(message);
}


void setup() {
  Serial.begin(115200);
  ubidots.setDebug(true);
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  pinMode(LED_GREEN_PIN_1, OUTPUT);
  pinMode(LED_YELLOW_PIN_1, OUTPUT);
  pinMode(LED_RED_PIN_1, OUTPUT);
  pinMode(LED_GREEN_PIN_2, OUTPUT);
  pinMode(LED_YELLOW_PIN_2, OUTPUT);
  pinMode(LED_RED_PIN_2, OUTPUT);

  timer = millis();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LDR_CALCADA_PIN, INPUT);
  pinMode(LDR_RUA_PIN, INPUT);
}

void loop() {  
  readDistance();
  readLight();
  delay(100);

  if (!ubidots.connected()) {
    ubidots.reconnect();
  }

  controlTrafficLight();

  if (millis() - timer >= PUBLISH_FREQUENCY) {
    publishData();
    timer = millis();
  }

  ubidots.loop();
}

void readLight() {
  luz_calcada = analogRead(LDR_CALCADA_PIN);
  luz_rua = analogRead(LDR_RUA_PIN);
  Serial.print("Luz calçada: ");
  Serial.println(luz_calcada);
  Serial.print("Luz rua: ");
  Serial.println(luz_rua);
}

void readDistance() {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration_us = pulseIn(ECHO_PIN, HIGH);
  distance_cm = 0.017 * duration_us;

  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
}

void controlTrafficLight() {
  // Modo Noturno: se a luz da calçada for menor que 1000
  if (luz_calcada < 1000) {
    // Pisca amarelo intermitente para ambos os semáforos
    setTrafficLightYellowBlinking();
  } 
  else if (luz_rua >= 2000) {
    // Quando a luz da rua for maior ou igual a 2000, semáforo 1 funciona normalmente
    if (isGreen) {
      // Semáforo 1 fica verde, semáforo 2 fica vermelho
      setTrafficLight(LOW, HIGH, LOW);  // Semáforo 1 vai para o amarelo antes de vermelho
      setTrafficLight2(LOW, LOW, HIGH); // Semáforo 2 fica vermelho
      yellowStartTime = millis();
      isGreen = false;
      isYellow = true;
    } 
    else if (isYellow && millis() - yellowStartTime >= 3000) {
      // Após 3 segundos de amarelo, semáforo 1 vai para o vermelho
      setTrafficLight(LOW, LOW, HIGH);
      setTrafficLight2(LOW, LOW, HIGH); // Semáforo 2 permanece vermelho
      isYellow = false;
    } 
    else if (!isYellow) {
      // Se semáforo 1 ainda não estava verde, ele fica vermelho
      setTrafficLight(LOW, LOW, HIGH);
      setTrafficLight2(HIGH, LOW, LOW); // Semáforo 2 fica vermelho
    }
  } 
  else {
    // Semáforo 1 fica verde
    if (!isGreen) {
      setTrafficLight(HIGH, LOW, LOW);  // Semáforo 1 fica verde
      setTrafficLight2(LOW, LOW, HIGH); // Semáforo 2 fica vermelho enquanto o semáforo 1 estiver verde
      greenStartTime = millis();
      isGreen = true;
    }

    // Semáforo 1 permanece verde por 6 segundos, depois verifica a distância
    if (millis() - greenStartTime >= 6000) {
      if (distance_cm < 4.5) {
        // Se a distância for menor que 4,5 cm, semáforo 2 permanece vermelho
        setTrafficLight2(LOW, LOW, HIGH); // Semáforo 2 fica vermelho
        setTrafficLight(HIGH, LOW, LOW);  // Semáforo 1 fica verde
      } else {
        // Se a distância for maior que 4,5 cm, semáforo 2 pode ficar verde
        setTrafficLight2(HIGH, LOW, LOW); // Semáforo 2 fica verde
        setTrafficLight(HIGH, LOW, HIGH);  // Semáforo 1 fica vermelho
        isGreen = false;
      }
    }
  }
}

// Função para piscar o amarelo no modo noturno
void setTrafficLightYellowBlinking() {
  unsigned long currentTime = millis();
  
  // Pisca ambos os semáforos em amarelo intermitente
  bool yellowState = (currentTime / 500) % 2;  // Pisca a cada 500ms
  setTrafficLight(LOW, yellowState, LOW);      // Semáforo 1 pisca amarelo
  setTrafficLight2(LOW, yellowState, LOW);     // Semáforo 2 pisca amarelo
  isGreen = false;
  isYellow = false;
}

// Função para controlar o semáforo 2
void setTrafficLight2(bool green, bool yellow, bool red) {
  digitalWrite(LED_GREEN_PIN_2, green);
  digitalWrite(LED_YELLOW_PIN_2, yellow);
  digitalWrite(LED_RED_PIN_2, red);
}

void setTrafficLight(bool green, bool yellow, bool red) {
  digitalWrite(LED_GREEN_PIN_1, green);
  digitalWrite(LED_YELLOW_PIN_1, yellow);
  digitalWrite(LED_RED_PIN_1, red);
}

void publishData() {
  bool red_light = digitalRead(LED_RED_PIN_1);
  bool yellow_light = digitalRead(LED_YELLOW_PIN_1);
  bool green_light = digitalRead(LED_GREEN_PIN_1);

  Serial.println(red_light);
  Serial.println(yellow_light);
  Serial.println(green_light);

  ubidots.add(VARIABLE_LABEL_RED_1, red_light);
  ubidots.publish(DEVICE_LABEL);

  ubidots.add(VARIABLE_LABEL_YELLOW_1, yellow_light);
  ubidots.publish(DEVICE_LABEL);

  ubidots.add(VARIABLE_LABEL_GREEN_1, green_light);
  ubidots.publish(DEVICE_LABEL);

  if (luz_rua < 2000) {
    ubidots.add(VARIABLE_LABEL_LDR_RUA, 1);
    ubidots.publish(DEVICE_LABEL);
  } else {
    ubidots.add(VARIABLE_LABEL_LDR_RUA, 0);
    ubidots.publish(DEVICE_LABEL);
  }

  if (luz_calcada < 1000) {
    ubidots.add(VARIABLE_LABEL_LDR_CALCADA, 1);
    ubidots.publish(DEVICE_LABEL);
  } else {
    ubidots.add(VARIABLE_LABEL_LDR_CALCADA, 0);
    ubidots.publish(DEVICE_LABEL);
  }

  ubidots.add(VARIABLE_LABEL_DISTANCE, distance_cm);
  ubidots.publish(DEVICE_LABEL);
}
