#include <Arduino.h>

#define RXD2 16
#define TXD2 17

HardwareSerial Radar(2);

// Función para enviar comando al radar
void enviarComando(const uint8_t* cmd, size_t len) {
  Radar.write(cmd, len);
}

// Función para mostrar la respuesta en HEX
void mostrarRespuesta() {
  while (Radar.available()) {
    uint8_t b = Radar.read();
    if (b < 16) Serial.print("0");
    Serial.print(b, HEX);
    Serial.print(" ");
  }
  Serial.println();
}

// Función para leer la resolución y mostrarla en texto
void leerResolucion() {
  uint8_t cmdRes[] = {0xFD,0xFC,0xFB,0xFA,0x02,0x00,0x11,0x00,0x04,0x03,0x02,0x01};
  enviarComando(cmdRes, sizeof(cmdRes));
  delay(500); // espera breve para la respuesta

  // Leer los bytes de respuesta
  if (Radar.available() >= 12) { // Ajusta según tamaño del ACK
    uint8_t buf[20];
    int len = 0;
    while (Radar.available()) {
      buf[len++] = Radar.read();
      if (len >= 20) break;
    }

    // Buscar el byte de resolución en el ACK
    // En el datasheet es el primer byte después del comando 0x11
    for (int i = 0; i < len - 1; i++) {
      if (buf[i] == 0x11) {
        uint8_t res = buf[i + 1];
        Serial.print("Resolución actual: ");
        switch(res) {
          case 0: Serial.println("75 cm"); break;
          case 1: Serial.println("50 cm"); break;
          case 3: Serial.println("20 cm"); break;
          default: Serial.print("Desconocida (0x"); Serial.print(res, HEX); Serial.println(")"); break;
        }
        return;
      }
    }
    Serial.println("No se encontró el byte de resolución en la respuesta.");
  } else {
    Serial.println("No hay datos de respuesta disponibles.");
  }
}

void setup() {
  Serial.begin(115200);
  Radar.begin(115200, SERIAL_8N1, RXD2, TXD2);
  delay(3000);

  Serial.println("Consulta de configuración del radar...");

  // Leer resolución actual
  Serial.println("Leyendo resolución de distancia...");
  leerResolucion();

  // Opcional: leer parámetros básicos
  uint8_t cmdBase[] = {0xFD,0xFC,0xFB,0xFA,0x02,0x00,0x12,0x00,0x04,0x03,0x02,0x01};
  Serial.println("Enviando comando: Leer parámetros básicos");
  enviarComando(cmdBase, sizeof(cmdBase));
  delay(3000);
  mostrarRespuesta();

  Serial.println("Consulta completada.");
}

void loop() {
  // No hacemos nada en el loop
}
