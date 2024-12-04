#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Definindo as linhas e colunas do teclado
const byte ROW_NUM    = 4; // número de linhas do teclado
const byte COLUMN_NUM = 4; // número de colunas do teclado

// Mapeamento das teclas do teclado 4x4
char keys[ROW_NUM][COLUMN_NUM] = {
  {'D','C','B','A'},  // Linha 1
  {'#','9','6','3'},  // Linha 2
  {'0','8','5','2'},  // Linha 3
  {'*','7','4','1'}   // Linha 4
};

// Pinos de conexão para as linhas e colunas
byte pin_rows[ROW_NUM] = {2, 3, 4, 5};   // Linhas conectadas aos pinos 2 a 5
byte pin_column[COLUMN_NUM] = {6, 7, 8, 9}; // Colunas conectadas aos pinos 6 a 9

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

// Definindo o display LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço do LCD (pode ser 0x3F ou 0x27)

// Definindo o servo motor
Servo catraca;
int pos = 0;

// Senha predefinida
const String senhaCorreta = "1234";
String senhaDigitada = "";

void setup() {
  lcd.begin(16, 2);         // Inicializa o display LCD
  lcd.setBacklight(1);      // Liga o backlight do display
  lcd.setCursor(0, 0);      // Posiciona o cursor
  lcd.print("Senha: ");      // Exibe "Senha:"

  catraca.attach(12);       // Conecta o servo ao pino 12
  catraca.write(0);         // Posiciona o servo na posição inicial

  delay(1000); // Espera 1 segundo

  Serial.begin(9600); // Inicia a comunicação serial
  Serial.println("Iniciando sistema...");
}

void loop() {
  char tecla = keypad.getKey(); // Lê a tecla pressionada

  if (tecla) {
    Serial.print("Tecla pressionada: ");
    Serial.println(tecla);  // Imprime a tecla pressionada no monitor serial

    // Adiciona um pequeno delay para evitar leituras duplicadas
    delay(50);

    // Se o botão '*' for pressionado, verifica a senha
    if (tecla == '*') { 
      lcd.setCursor(0, 0);
      lcd.print("Verificando...");  // Exibe a mensagem de verificação
      delay(500);  // Aguarda um pouco antes de verificar

      Serial.println("Verificando senha...");
      if (senhaDigitada == senhaCorreta) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Senha Correta!");
        catraca.write(90); // Move o servo para liberar a catraca
        delay(2000); // Aguarda 2 segundos
        catraca.write(0); // Retorna o servo à posição inicial
        senhaDigitada = ""; // Limpa a senha digitada
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Senha Errada!");
        delay(2000); // Aguarda 2 segundos para exibir a mensagem
        senhaDigitada = ""; // Limpa a senha digitada
      }

      // Após a verificação, exibe novamente a tela inicial de senha
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Senha: ");
      lcd.setCursor(0, 1);
      lcd.print(senhaDigitada); // Exibe a senha digitada
    } else if (tecla == '#') { // Quando o botão '#' for pressionado, limpa a senha
      senhaDigitada = ""; // Apaga a senha digitada
      lcd.setCursor(0, 0);
      lcd.print("Senha: ");
      lcd.setCursor(0, 1);
      lcd.print(senhaDigitada); // Exibe a senha vazia
    } else if (tecla != '*') { // Não registra o '*' e '#' para digitação
      if (senhaDigitada.length() < 4) { // Limita a senha a 4 dígitos
        senhaDigitada += tecla; // Adiciona o dígito à senha digitada
        lcd.setCursor(0, 1);   // Posiciona o cursor para a linha 1, coluna 0
        lcd.print(senhaDigitada); // Exibe a senha digitada
      }
    }
  }
}
