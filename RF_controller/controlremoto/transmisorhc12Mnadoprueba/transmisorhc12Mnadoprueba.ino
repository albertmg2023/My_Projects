#include <HardwareSerial.h>

// Pines de botones
const int BTN_ARRIBA = 25;

// HC-12 en UART2 (TX=18, RX=19)
HardwareSerial hc12(2);

void setup() {
  pinMode(BTN_ARRIBA, INPUT_PULLUP);  // botón arriba
  hc12.begin(9600, SERIAL_8N1, 19, 18); // RX, TX
}

void loop() {
  if(digitalRead(BTN_ARRIBA) == LOW){
    hc12.println(118); // enviar código 118
    delay(200);        // antirrebote simple
  }
}
