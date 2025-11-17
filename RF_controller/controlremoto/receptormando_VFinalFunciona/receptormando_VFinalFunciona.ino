#include <U8g2lib.h>
#include <Wire.h>
#include <HardwareSerial.h>

// Pantalla OLED SSD1306 128x64 I2C
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// HC-12 en pines 18 (TX) y 19 (RX)
HardwareSerial hc12(1); // usar Serial1
const long baudRate = 9600;

// Buffer de recepción
char incomingMessage[128]; // tamaño máximo de mensaje
int idx = 0;

// Para controlar duración de mensaje en pantalla
unsigned long lastReceivedTime = 0;
const unsigned long displayDuration = 5000; // 5 segundos
bool messageOnScreen = false;

// --------------------------------------------------
// Setup
// --------------------------------------------------
void setup() {
  // Inicializar pantalla
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 15, "Receptor iniciado...");
  u8g2.sendBuffer();

  // Inicializar HC-12 (RX=19, TX=18)
  hc12.begin(baudRate, SERIAL_8N1, 19, 18);
}

// --------------------------------------------------
// Loop principal
// --------------------------------------------------
void loop() {
  // Leer datos del HC-12
  while (hc12.available()) {
    char c = hc12.read();
    if (c == '\n' || c == '\r') {
      if (idx > 0) {
        incomingMessage[idx] = '\0'; // terminar string
        displayMessage(incomingMessage);
        lastReceivedTime = millis();
        messageOnScreen = true;
        idx = 0; // reiniciar buffer
      }
    } else if (idx < sizeof(incomingMessage) - 1) {
      incomingMessage[idx++] = c;
    }
  }

  // Borrar mensaje después de displayDuration
  if (messageOnScreen && millis() - lastReceivedTime > displayDuration) {
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    messageOnScreen = false;
  }
}

// --------------------------------------------------
// Función para mostrar mensaje en pantalla 128x64
// --------------------------------------------------
void displayMessage(const char* msg) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);

  int lineHeight = 12;   // altura entre líneas
  int maxChars = 16;     // 128px / 8px ≈ 16 caracteres por línea
  int maxLines = 5;      // 64px / 12px ≈ 5 líneas

  for (int i = 0; i < maxLines; i++) {
    int start = i * maxChars;
    if (msg[start] == '\0') break; // fin del mensaje
    char line[maxChars + 1];
    int j;
    for (j = 0; j < maxChars && msg[start + j] != '\0'; j++) {
      line[j] = msg[start + j];
    }
    line[j] = '\0';
    u8g2.drawStr(0, (i + 1) * lineHeight, line);
  }

  u8g2.sendBuffer();
}
