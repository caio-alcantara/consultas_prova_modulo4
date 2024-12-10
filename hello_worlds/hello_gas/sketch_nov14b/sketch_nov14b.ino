#define ANALOG_PIN 34  // Pino analógico do sensor de gás (A34)
#define DIGITAL_PIN 35 // Pino digital do sensor de gás (D35)

void setup() {
  Serial.begin(115200);       // Inicializa a comunicação serial
  pinMode(DIGITAL_PIN, INPUT); // Define o pino digital como entrada
}

void loop() {
  // Lê o valor analógico do sensor MQ-135
  int analogValue = analogRead(ANALOG_PIN);
  Serial.print("Valor Analogico: ");
  Serial.println(analogValue);

  // Lê o valor digital do sensor MQ-135
  int digitalValue = digitalRead(DIGITAL_PIN);
  Serial.print("Valor Digital: ");
  Serial.println(digitalValue);

  // Exibe um aviso caso o valor digital indique presença de gás
  if (digitalValue == HIGH) {
    Serial.println("Alerta: Presença de gás detectada!");
  } else {
    Serial.println("Nível de gás normal.");
  }

  delay(1000); // Aguarda 1 segundo antes de realizar a próxima leitura
}
