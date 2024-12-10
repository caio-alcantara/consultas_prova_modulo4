#include <ESP32Servo.h>

#define SERVO_PIN 33  // Pino de controle do servo

Servo myServo; // Cria um objeto Servo para controlar o servo

void setup() {
  Serial.begin(115200); // Inicializa a comunicação serial
  myServo.attach(SERVO_PIN); // Conecta o servo ao pino 33 do ESP32
}

void loop() {
  // Move o servo de 0 a 180 graus
  for (int angle = 0; angle <= 180; angle += 10) {
    myServo.write(angle);  // Define o ângulo do servo
    Serial.print("Ângulo: ");
    Serial.println(angle);
    delay(500); // Espera 500 ms entre cada posição
  }

  delay(1000); // Aguarda 1 segundo antes de inverter o movimento

  // Move o servo de 180 a 0 graus
  for (int angle = 180; angle >= 0; angle -= 10) {
    myServo.write(angle);  // Define o ângulo do servo
    Serial.print("Ângulo: ");
    Serial.println(angle);
    delay(500); // Espera 500 ms entre cada posição
  }

  delay(1000); // Aguarda 1 segundo antes do próximo ciclo
}
