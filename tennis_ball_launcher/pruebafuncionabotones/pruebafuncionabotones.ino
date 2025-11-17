#include <Wire.h>
#include <U8g2lib.h>

// Configuración de la pantalla OLED
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// Pines de los botones (finales de carrera)
const int BOTON_VA = 12;  // Vertical horario
const int BOTON_VB = 13;  // Vertical antihorario
const int BOTON_HA = 15;  // Horizontal horario
const int BOTON_HB = 17;   // Horizontal antihorario

void setup() {
  // Inicialización de la pantalla OLED
  Wire.begin(21, 22);
  u8g2.begin();

  // Configuración de los pines de los botones como INPUT_PULLUP
  pinMode(BOTON_VA, INPUT_PULLUP);
  pinMode(BOTON_VB, INPUT_PULLUP);
  pinMode(BOTON_HA, INPUT_PULLUP);
  pinMode(BOTON_HB, INPUT_PULLUP);
}

void loop() {
  // Leer estado de los botones (LOW = presionado)
  bool estadoVA = digitalRead(BOTON_VA) == LOW;
  bool estadoVB = digitalRead(BOTON_VB) == LOW;
  bool estadoHA = digitalRead(BOTON_HA) == LOW;
  bool estadoHB = digitalRead(BOTON_HB) == LOW;

  // Mostrar el estado en la pantalla OLED
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_5x8_tr);
  u8g2.drawStr(0, 10, "Finales de carrera:");

  u8g2.setCursor(0, 25);
  u8g2.print("V-Horario: ");
  u8g2.print(estadoVA ? "Presionado" : "Liberado");

  u8g2.setCursor(0, 35);
  u8g2.print("V-Antihor: ");
  u8g2.print(estadoVB ? "Presionado" : "Liberado");

  u8g2.setCursor(0, 45);
  u8g2.print("H-Horario: ");
  u8g2.print(estadoHA ? "Presionado" : "Liberado");

  u8g2.setCursor(0, 55);
  u8g2.print("H-Antihor: ");
  u8g2.print(estadoHB ? "Presionado" : "Liberado");

  u8g2.sendBuffer();

  delay(100); // Pequeño retardo para evitar parpadeo
}
