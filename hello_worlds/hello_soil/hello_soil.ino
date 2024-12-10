#define SOIL_SENSOR_PIN 25  // Pino analógico do sensor de umidade do solo (A25)

void setup() {
  Serial.begin(115200);      // Inicializa a comunicação serial
  pinMode(SOIL_SENSOR_PIN, INPUT); // Define o pino do sensor como entrada
}

void loop() {
  // Lê o valor analógico do sensor de umidade do solo
  int soilMoistureValue = analogRead(SOIL_SENSOR_PIN);

  // Converte o valor para uma porcentagem (opcional, para facilitar a leitura)
  int moisturePercent = map(soilMoistureValue, 4095, 0, 0, 100); 

  // Imprime o valor no Serial Monitor
  Serial.print("Valor de umidade do solo: ");
  Serial.print(soilMoistureValue);
  Serial.print(" | Umidade em %: ");
  Serial.print(moisturePercent);
  Serial.println("%");

  delay(1000);  // Aguarda 1 segundo antes de realizar a próxima leitura
}
