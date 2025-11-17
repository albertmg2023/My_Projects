#include <Arduino.h>

#define RXD2 16
#define TXD2 17

HardwareSerial Radar(2);

struct Target {
  uint8_t state;
  uint16_t moveDist;   // distancia en cm
  uint8_t moveEnergy;
  uint16_t staticDist;
  uint8_t staticEnergy;
};

Target currentTarget;
Target prevTarget;

// Buffer de lectura
#define BUF_SIZE 256
uint8_t buf[BUF_SIZE];
uint8_t bufIndex = 0;

// Tiempo de la última trama para calcular velocidad
unsigned long lastTime = 0;

void setup() {
  Serial.begin(115200);
  Radar.begin(115200, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Radar iniciado, esperando tramas...");

  // Comando para seleccionar resolución máxima = 75cm/distance gate
  uint8_t cmdResMaxFPS[] = {0xFD,0xFC,0xFB,0xFA, 0x02,0x00, 0x11,0x00, 0x00,0x03,0x02,0x01};
  Radar.write(cmdResMaxFPS, sizeof(cmdResMaxFPS));
}

void parseRadarFrame() {
  if (bufIndex < 10) return; // no hay trama completa aún

  for (int i = 0; i < bufIndex - 4; i++) {
    // Buscar header
    if (buf[i] == 0xF4 && buf[i+1] == 0xF3 && buf[i+2] == 0xF2 && buf[i+3] == 0xF1) {
      int len = buf[i+4] + (buf[i+5] << 8); // longitud intra-frame
      if (i + 6 + len + 4 > bufIndex) return; // trama incompleta

      uint8_t *data = &buf[i+6];

      if (data[0] == 0x02) { // modo normal (basic target info)
        // Parsear datos (little endian)
        currentTarget.state = data[2];
        currentTarget.moveDist = data[3] + (data[4]<<8);
        currentTarget.moveEnergy = data[5];
        currentTarget.staticDist = data[6] + (data[7]<<8);
        currentTarget.staticEnergy = data[8];

        // Calcular velocidad solo para objetos en movimiento
        if (currentTarget.state == 1 || currentTarget.state == 3) {
          unsigned long now = millis();
          float dt_s = (now - lastTime) / 1000.0;
          lastTime = now;

          // velocidad = diferencia de distancia / tiempo entre tramas
          float velocity_cm_s = float(currentTarget.moveDist - prevTarget.moveDist) / dt_s;
          float velocity_kmh = velocity_cm_s * 0.036;

          prevTarget = currentTarget;

          // Mostrar solo si velocidad > 5 km/h
          if (abs(velocity_kmh) > 5.0) {
            Serial.print("Velocidad aprox: "); 
            Serial.print(velocity_kmh, 1); 
            Serial.println(" km/h");
          }
        }
      }

      // Mover bytes restantes al inicio del buffer
      int remaining = bufIndex - (i + 6 + len + 4);
      memmove(buf, &buf[i + 6 + len + 4], remaining);
      bufIndex = remaining;
      return;
    }
  }
  // Si no se encontró header válido, desplazar 1 byte
  if(bufIndex >= BUF_SIZE) bufIndex = 0;
}

void loop() {
  while (Radar.available() && bufIndex < BUF_SIZE) {
    buf[bufIndex++] = Radar.read();
  }
  parseRadarFrame();
}
