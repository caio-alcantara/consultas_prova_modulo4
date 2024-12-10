int relePin = 7; // Pino digital conectado ao relé

void setup() {
  pinMode(relePin, OUTPUT); // Configura o pino como saída
  Serial.begin(9600);
}

void loop() {
  digitalWrite(relePin, HIGH); // Liga o relé
  Serial.println("Relé ligado");
  delay(1000); // Espera 1 segundo
  digitalWrite(relePin, LOW); // Desliga o relé
  Serial.println("Relé desligado");
  delay(1000); // Espera 1 segundo
}
