#define BUTTON_PIN 15  // Pino onde o botão está conectado

void setup() {
  Serial.begin(115200);                   // Inicializa a comunicação serial
  pinMode(BUTTON_PIN, INPUT_PULLUP);      // Define o pino do botão como entrada com resistor pull-up
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);  // Lê o estado do botão

  // Verifica se o botão está pressionado
  if (buttonState == LOW) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão não pressionado.");
  }

  delay(500);  // Aguarda 500 ms antes de ler o estado novamente
}
