class Timer {
private:
  unsigned long interval;  // Intervalo do timer em milissegundos
  unsigned long lastTime;  // Última vez que o timer foi acionado
  bool enabled;            // Indica se o timer está ativo

public:
  // Construtor: inicializa o timer
  Timer(unsigned long intervalMillis) {
    interval = intervalMillis;
    lastTime = 0;
    enabled = true;
  }

  // Configura o intervalo do timer
  void setInterval(unsigned long intervalMillis) {
    interval = intervalMillis;
  }

  // Inicia ou reinicia o timer
  void start() {
    lastTime = millis();
    enabled = true;
  }

  // Para o timer
  void stop() {
    enabled = false;
  }

  // Verifica se o tempo decorrido atingiu o intervalo
  bool isReady() {
    if (!enabled) return false;

    unsigned long currentTime = millis();
    if (currentTime - lastTime >= interval) {
      lastTime = currentTime; // Reinicia o contador
      return true;
    }
    return false;
  }
};

Timer timer1(1000); // Timer de 1 segundo

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Verifica se o timer está pronto
  if (timer1.isReady()) {
    Serial.println("1 segundo se passou!");
  }

  // Outras tarefas podem ser executadas aqui sem bloqueio
}
