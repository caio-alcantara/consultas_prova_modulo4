#define TRIG_PIN 18  // Pino TRIG do sensor
#define ECHO_PIN 19  // Pino ECHO do sensor

void setup() {
  Serial.begin(115200);       // Inicializa a comunicação serial
  pinMode(TRIG_PIN, OUTPUT);  // Define o pino TRIG como saída
  pinMode(ECHO_PIN, INPUT);   // Define o pino ECHO como entrada
}

void loop() {
  // Envia um pulso de 10 microssegundos no TRIG
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Lê a duração do pulso no pino ECHO
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calcula a distância em cm (velocidade do som é ~34300 cm/s)
  float distance = (duration * 0.0343) / 2;

  // Imprime a distância no Serial Monitor
  Serial.print("Distância: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(200);  // Espera 500ms antes de medir novamente
}
