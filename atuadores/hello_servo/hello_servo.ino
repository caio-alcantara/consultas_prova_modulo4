#include <Servo.h>

Servo servoMotor; // Cria um objeto servo
int servoPin = 9; // Pino conectado ao sinal do servo

void setup() {
  servoMotor.attach(servoPin); // Conecta o servo ao pino
}

void loop() {
  servoMotor.write(0); // Gira o servo para 0 graus
  delay(1000); // Espera 1 segundo
  servoMotor.write(90); // Gira o servo para 90 graus
  delay(1000); // Espera 1 segundo
  servoMotor.write(180); // Gira o servo para 180 graus
  delay(1000); // Espera 1 segundo
}
