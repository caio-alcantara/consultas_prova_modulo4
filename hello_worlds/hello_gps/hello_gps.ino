#include <TinyGPS++.h>
#include <HardwareSerial.h>

// Instancia o objeto TinyGPS++
TinyGPSPlus gps;

// Configura a Serial2 para o GPS (definindo RX e TX nos pinos 16 e 17)
HardwareSerial SerialGPS(2);

void setup() {
  // Inicializa a comunicação serial com o computador (Monitor Serial)
  Serial.begin(115200);
  
  // Inicializa a Serial2 com baud rate de 9600 (padrão do GPS)

  //CONEXOES:
  // RX SAINDO DO GPS VAI PARA 17 do ESP
  // TX SAINDO DO GPS VAI PARA 16 do ESP
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17); // CONECTA RX MODULO - 17; TX MODULO - 16

  Serial.println("Iniciando GPS...");
}

void loop() {
  while (SerialGPS.available() > 0) {
    char c = SerialGPS.read();
    gps.encode(c);  // Processa os dados recebidos

    // Verifica se a localização é válida
    if (gps.location.isValid()) {
      Serial.println("Informações do GPS:");

      // Latitude e Longitude
      Serial.print("Coordenadas: ");
      Serial.print(gps.location.lat(), 6);  // 6 dígitos de precisão
      Serial.print(", ");
      Serial.print(gps.location.lng(), 6);  // 6 dígitos de precisão
      Serial.println();

      // Altitude
      Serial.print("Altitude: ");
      Serial.print(gps.altitude.meters());
      Serial.println(" m");

      // Número de Satélites
      Serial.print("Número de Satélites: ");
      Serial.println(gps.satellites.value());

      // Velocidade
      Serial.print("Velocidade: ");
      Serial.print(gps.speed.knots());
      Serial.println(" nós");

      // Direção
      Serial.print("Direção: ");
      Serial.print(gps.course.deg());
      Serial.println(" graus");

      // Data e Hora
      Serial.print("Data: ");
      if (gps.date.isValid()) {
        Serial.print(gps.date.day());
        Serial.print("/");
        Serial.print(gps.date.month());
        Serial.print("/");
        Serial.print(gps.date.year());
      } else {
        Serial.print("INVÁLIDA");
      }
      Serial.println();

      Serial.print("Hora: ");
      if (gps.time.isValid()) {
        int hour = gps.time.hour();
        int minute = gps.time.minute();
        int second = gps.time.second();

        // Ajuste a hora para o horário de Brasília (UTC-3)
        hour = (hour + 21) % 24;  // UTC - 3 -> (21 + 24 - 3) % 24

        if (hour < 10) Serial.print("0");
        Serial.print(hour);
        Serial.print(":");
        if (minute < 10) Serial.print("0");
        Serial.print(minute);
        Serial.print(":");
        if (second < 10) Serial.print("0");
        Serial.print(second);
      } else {
        Serial.print("INVÁLIDA");
      }
      Serial.println();

      Serial.println();
    } else {
      Serial.println("Localização não válida.");
    }
  }
}
