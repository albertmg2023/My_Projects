#include <Wire.h>

void setup() {
  Serial.begin(115200);
  // Pines por defecto en ESP32: SDA=21, SCL=22
  Wire.begin(21, 22);  
  Serial.println("\nI2C Scanner - ESP32");
}

void loop() {
  byte error, address;
  int nDevices = 0;

  Serial.println("Escaneando...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Dispositivo I2C encontrado en 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      nDevices++;
    }
  }

  if (nDevices == 0) Serial.println("No se encontraron dispositivos I2C\n");
  else Serial.println("Hecho.\n");

  delay(2000); // espera 2 segundos antes de volver a escanear
}
