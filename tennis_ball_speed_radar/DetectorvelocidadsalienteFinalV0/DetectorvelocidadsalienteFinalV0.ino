#include <HardwareSerial.h>

HardwareSerial RadarSerial(1);  // Usamos UART1 del ESP32

#define RADAR_RX 18  // ESP32 RX <- HLK TX
#define RADAR_TX 19  // ESP32 TX -> HLK RX

String buffer = "";

void setup() {
  Serial.begin(115200);
  RadarSerial.begin(9600, SERIAL_8N1, RADAR_RX, RADAR_TX);

  Serial.println("HLK-LD2415H - Solo objetos alejándose (>6 km/h) @20 fps");

  // --- Configurar radar ---
  // Formato: 43 46 02 [direccion] [fps] [unidad] 0D 0A
  // direccion = 0x02 (solo going)
  // fps = 0x00 (~22 fps, aprox 20 fps)
  // unidad = 0x00 (km/h)
  uint8_t cmd_onlyGoing20fps[] = {0x43, 0x46, 0x02, 0x02, 0x00, 0x00, 0x0D, 0x0A};
  RadarSerial.write(cmd_onlyGoing20fps, sizeof(cmd_onlyGoing20fps));
  delay(100);
}

void loop() {
  while (RadarSerial.available()) {
    char c = RadarSerial.read();
    buffer += c;

    if (buffer.endsWith("\r\n")) {
      if (buffer.startsWith("V")) {
        char dir = buffer[1];  // '+' o '-'
        String number = buffer.substring(2, buffer.length() - 2);
        float speed = number.toFloat();

        // Mostrar solo velocidades mayores a 6 km/h
        if (speed > 6.0) {
          Serial.print("Velocidad: ");
          Serial.print(speed);
          Serial.print(" km/h ");
          if (dir == '+') Serial.println("(acercándose)"); // ya no debería salir
          else Serial.println("(alejándose)");
        }
      }
      buffer = "";
    }
  }
}
