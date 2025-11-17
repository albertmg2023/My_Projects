#include <U8g2lib.h>
#include <Wire.h>

// Pantalla OLED SSD1306 128x32
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

// Botones
const int BTN_CHANGE = 32;  
const int BTN_START  = 33;  

// Lista de pines de prueba
int availablePins[] = {12, 14, 27, 26,23, 19, 18, 15, 16,17};
int totalPins = sizeof(availablePins) / sizeof(availablePins[0]);
int selectedIndex = 0;
int testPin = -1;   // Pin elegido (inicia sin selección)

void setup() {
  display.begin();
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB08_tr);
  display.drawStr(0, 15, "Selecciona PIN...");
  display.sendBuffer();

  pinMode(BTN_CHANGE, INPUT_PULLUP);
  pinMode(BTN_START, INPUT_PULLUP);
}

void loop() {
  // ----------- Selección de pin -----------
  while (testPin == -1) {
    display.clearBuffer();
    display.setFont(u8g2_font_ncenB08_tr);
    display.drawStr(0, 12, "Selecciona PIN:");
    display.setCursor(0, 28);
    display.print("PIN: ");
    display.print(availablePins[selectedIndex]);
    display.sendBuffer();

    if (digitalRead(BTN_CHANGE) == LOW) {
      selectedIndex = (selectedIndex + 1) % totalPins;
      delay(250);
    }
    if (digitalRead(BTN_START) == LOW) {
      testPin = availablePins[selectedIndex];  // Guardar pin elegido
      delay(250);
    }
  }

  // --------- MODO SALIDA (10 segundos) ----------
  unsigned long startTime = millis();
  pinMode(testPin, OUTPUT);

  while (millis() - startTime < 10000) { // 10 segundos
    display.clearBuffer();
    display.setFont(u8g2_font_ncenB08_tr);
    display.drawStr(0, 12, "Modo: SALIDA");
    display.setCursor(0, 28);
    display.print("PIN ");
    display.print(testPin);
    display.sendBuffer();

    digitalWrite(testPin, HIGH);
    delay(1000);
    digitalWrite(testPin, LOW);
    delay(1000);
  }

  // --------- MODO ENTRADA (10 segundos) ----------
  startTime = millis();
  pinMode(testPin, INPUT_PULLUP);

  while (millis() - startTime < 10000) { // 10 segundos
    int estado = digitalRead(testPin);

    display.clearBuffer();
    display.setFont(u8g2_font_ncenB08_tr);
    display.drawStr(0, 12, "Modo: ENTRADA");
    display.setCursor(0, 28);
    if (estado == LOW) {
      display.print("Boton PRESIONADO");
    } else {
      display.print("Boton LIBRE");
    }
    display.sendBuffer();

    delay(200);
  }
}
