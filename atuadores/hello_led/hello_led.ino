int ledPin = 13; // Pino digital conectado ao LED

void setup() {
  pinMode(ledPin, OUTPUT); // Configura o pino como saída
}

void loop() {
  digitalWrite(ledPin, HIGH); // Liga o LED
  delay(500); // Espera 500 ms
  digitalWrite(ledPin, LOW); // Desliga o LED
  delay(500); // Espera 500 ms
}
