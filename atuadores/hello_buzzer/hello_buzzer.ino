int buzzerPin = 8; // Pino conectado ao buzzer

void setup() {
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  digitalWrite(buzzerPin, HIGH); // Liga o buzzer
  delay(500); // Emite som por 500 ms
  digitalWrite(buzzerPin, LOW); // Desliga o buzzer
  delay(500); // Pausa por 500 ms
}
