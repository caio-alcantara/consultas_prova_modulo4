#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o LCD no endereço I2C (ajuste o endereço se necessário)
LiquidCrystal_I2C lcd(0x27, 16, 2); // 0x27 é o endereço I2C para um LCD 16x2

void setup() {
  // Inicia a comunicação I2C e o LCD
  Wire.begin(21, 22);  // Configura SDA (21) e SCL (22) para o ESP32
  lcd.begin(16, 2);    // Define o tamanho do display (16 colunas e 2 linhas)
  lcd.init();          // Inicializa o display
  lcd.backlight();     // Ativa o backlight (luz de fundo)

  // Exemplo 1: Escrevendo uma mensagem simples na tela
  lcd.setCursor(0, 0); // Define o cursor na primeira linha, primeira coluna
  lcd.print("Hello, ESP32!");
  delay(2000);

  // Exemplo 2: Limpa o display e escreve em outra posição
  lcd.clear();         // Limpa o display
  lcd.setCursor(4, 0); // Define o cursor na primeira linha, quarta coluna
  lcd.print("Display");
  lcd.setCursor(3, 1); // Define o cursor na segunda linha, terceira coluna
  lcd.print("Test");
  delay(2000);

  // Exemplo 3: Apaga e ativa o cursor
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cursor example");
  lcd.cursor();        // Mostra o cursor
  delay(2000);
  lcd.noCursor();      // Esconde o cursor
  delay(2000);

  // Exemplo 4: Blink (piscando) do cursor
  lcd.blink();         // Cursor piscando
  lcd.setCursor(0, 1);
  lcd.print("Blinking cursor");
  delay(2000);
  lcd.noBlink();       // Para o piscar do cursor
  delay(2000);

  // Exemplo 5: Desativar e ativar o backlight
  lcd.clear();
  lcd.print("Backlight OFF");
  lcd.noBacklight();   // Desliga o backlight
  delay(2000);
  lcd.backlight();     // Liga o backlight
  lcd.clear();
  lcd.print("Backlight ON");
  delay(2000);

  // Exemplo 6: Deslocamento de texto para a esquerda
  lcd.clear();
  lcd.print("Scroll Left");
  delay(1000);
  for (int i = 0; i < 10; i++) {
    lcd.scrollDisplayLeft();  // Move o texto para a esquerda
    delay(300);
  }

  // Exemplo 7: Deslocamento de texto para a direita
  lcd.clear();
  lcd.print("Scroll Right");
  delay(1000);
  for (int i = 0; i < 10; i++) {
    lcd.scrollDisplayRight(); // Move o texto para a direita
    delay(300);
  }

  // Exemplo 8: Mostrar contagem
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Counter:");
  for (int i = 0; i <= 10; i++) {
    lcd.setCursor(9, 0); // Define a posição para o número da contagem
    lcd.print(i);
    delay(1000);
  }

  // Exemplo 9: Mensagem longa e truncamento
  lcd.clear();
  lcd.print("Mensagem longa");
  lcd.setCursor(0, 1);
  lcd.print("Truncamento demo");
  delay(3000);

  // Exemplo 10: Scroll automático de mensagem longa
  lcd.clear();
  lcd.print("Mensagem longa que precisa de scrolling");
  delay(2000);
  for (int i = 0; i < 20; i++) {
    lcd.scrollDisplayLeft();  // Scroll da mensagem longa
    delay(500);
  }
}

void loop() {
  // Não há necessidade de loop contínuo neste exemplo
}
