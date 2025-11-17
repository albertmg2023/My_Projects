#include <U8g2lib.h>
#include <Wire.h>

// Pantalla OLED SSD1306 128x32 por I2C
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

void setup() {
  // Asegurar pines correctos para ESP32
  Wire.begin(21, 22); // SDA=21, SCL=22

  display.begin();
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB08_tr);
  display.drawStr(0, 15, "Hola OLED!");
  display.sendBuffer();
}

void loop() {
  // Nada que hacer en loop, solo mostrar el texto
}
