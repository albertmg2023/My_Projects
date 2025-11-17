#include <HardwareSerial.h>
#include <BluetoothSerial.h>

HardwareSerial RadarSerial(1);  // UART1 para el radar
BluetoothSerial SerialBT;       // Bluetooth Serial



//tx es el 5o desde +12V
//Rx es el 6o
#define RADAR_RX 18  // ESP32 RX <- HLK TX
#define RADAR_TX 19  // ESP32 TX -> HLK RX

void setup() {
  //Serial.begin(115200);          // Monitor serial
  RadarSerial.begin(9600, SERIAL_8N1, RADAR_RX, RADAR_TX);
  SerialBT.begin("RadarESP32");
}

void loop() {
  // Cada trama del radar son 9 bytes
  if (RadarSerial.available() >= 9) {
    uint8_t data[9];
    RadarSerial.readBytes(data, 9);

    // Solo procesar si empieza con 'V' ASCII
    if (data[0] == 'V') {
      char dir = data[1];  // '+' = acercándose, '-' = alejándose
      char speedStr[6];    // XXX.XX
      memcpy(speedStr, &data[2], 6);
      speedStr[5] = '\0';
      float speed = atof(speedStr);

      // Mostrar en monitor serial
      //Serial.println(speed);

      // Enviar por Bluetooth
      SerialBT.println(speed);
    }
  }
}
