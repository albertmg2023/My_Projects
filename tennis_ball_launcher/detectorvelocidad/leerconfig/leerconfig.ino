#include <HardwareSerial.h>

HardwareSerial RadarSerial(1);  // UART1 del ESP32

#define RADAR_RX 13  // ESP32 RX <- Radar TX
#define RADAR_TX 14  // ESP32 TX -> Radar RX

String buffer = "";

void setup() {
  Serial.begin(115200);
  RadarSerial.begin(9600, SERIAL_8N1, RADAR_RX, RADAR_TX);

  delay(1000); // Pequeña espera antes de empezar

  // --- SALIR DEL MODO CSR ---
  uint8_t exitCSR[] = {0xFA, 0x31, 0x30, 0x30, 0xFB};
  RadarSerial.write(exitCSR, sizeof(exitCSR));
  Serial.println("Enviado comando para salir de CSR...");
  delay(5000); // esperar procesamiento

  // --- CONFIRMACION MODO ESTANDAR ---
  uint8_t confirmStandard[] = {0xFA, 0x55, 0xAA, 0xFF, 0xFB};
  RadarSerial.write(confirmStandard, sizeof(confirmStandard));
  Serial.println("Enviado comando de confirmación modo estándar.");
  delay(500);

  // --- CONFIGURAR RADAR ---
  // 43 46 01 [velocidad_minima] [angulo] [sensibilidad] 0D 0A
  uint8_t configRadar[] = {0x43, 0x46, 0x01, 0x0A, 0x0A, 0x03, 0x0D, 0x0A};
  // X1=10 km/h, X2=10 grados, X3=sensibilidad max (0x01)
  RadarSerial.write(configRadar, sizeof(configRadar));
  Serial.println("Configuración: velocidad_min=10km/h, angulo=10°, sensibilidad máxima.");

  delay(100);

  // --- SOLO OBJETOS ALEJÁNDOSE, 22 fps, km/h ---
  // 43 46 02 02 00 00 0D 0A
  uint8_t goingOnly[] = {0x43, 0x46, 0x02, 0x02, 0x00, 0x00, 0x0D, 0x0A};
  RadarSerial.write(goingOnly, sizeof(goingOnly));
  Serial.println("Configuración: solo detecta objetos alejándose, km/h.");
}

void loop() {
  while (RadarSerial.available()) {
    char c = RadarSerial.read();
    buffer += c;

    // Cada trama termina con \r\n
    if (buffer.endsWith("\r\n")) {
      if (buffer.startsWith("V")) {
        char dir = buffer[1]; // '+' o '-'
        String number = buffer.substring(2, buffer.length() - 2);
        float speed = number.toFloat();

        // Mostrar solo velocidades >= 10 km/h y alejándose
        if (speed >= 10.0 && dir == '-') {
          Serial.print("Velocidad detectada: ");
          Serial.print(speed);
          Serial.println(" km/h (alejándose)");
        }
      }
      buffer = "";
    }
  }
}
