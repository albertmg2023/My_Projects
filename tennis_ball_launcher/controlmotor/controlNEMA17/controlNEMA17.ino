#include <U8g2lib.h>
#include <Wire.h>

// Configuración de la pantalla OLED
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// Pines del joystick
const int VRX_PIN = 35;
const int VRY_PIN = 32;
const int SW_PIN  = 33;

// Pines de los botones(final de carrera)
const int BOTON_A = 12;  // Bloquea sentido horario
const int BOTON_B = 13;  // Bloquea sentido antihorario

// Pines del motor
const int dirPin  = 27;
const int stepPin = 14;

// Variables de control
float grados_ini = 0.0;
const int stepsPerRev = 200;     // Número de pasos por revolución del motor en modo full-step
const unsigned int pulseUs = 400; // Ancho de pulso seguro para DRV8825
int steps = 0;

// Variables de interrupción
volatile bool detenerGiroHorario = false;
volatile bool detenerGiroAntihorario = false;

// --- Interrupciones ---
void IRAM_ATTR interrupcionBotonA() {
  detenerGiroHorario = (digitalRead(BOTON_A) == LOW);  // LOW = pulsado (por INPUT_PULLUP)
}

void IRAM_ATTR interrupcionBotonB() {
  detenerGiroAntihorario = (digitalRead(BOTON_B) == LOW);
}

// --- Setup ---
void setup(void) {
  // Configuración de los pines
  pinMode(BOTON_A, INPUT_PULLUP);
  pinMode(BOTON_B, INPUT_PULLUP);
  pinMode(SW_PIN, INPUT_PULLUP);
  pinMode(VRX_PIN, INPUT);
  pinMode(VRY_PIN, INPUT);

  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);

  // Configuración de la pantalla OLED
  Wire.begin(21, 22);
  u8g2.begin();

  // Configuración de interrupciones (una sola por pin, con CHANGE)
  attachInterrupt(digitalPinToInterrupt(BOTON_A), interrupcionBotonA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BOTON_B), interrupcionBotonB, CHANGE);
}

// --- Mover motor ---
void moveDegrees(int dirPin, int stepPin, float grados) {
  if (grados == 0) return;

  // Bloquear si el botón correspondiente está pulsado
  if (grados > 0 && detenerGiroHorario) return;      
  if (grados < 0 && detenerGiroAntihorario) return;  

  // Configurar dirección
  digitalWrite(dirPin, grados > 0 ? HIGH : LOW);

  // Calcular pasos
  steps = abs((int)(fabs(grados) * stepsPerRev / 360.0 + 0.5));

  for (unsigned long i = 0; i < steps; i++) {
    if ((grados > 0 && detenerGiroHorario) || (grados < 0 && detenerGiroAntihorario)) {
      break;  // detener inmediatamente si el botón se presiona
    }
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(pulseUs);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(pulseUs);
  }

  // Actualizar ángulo
  grados_ini += grados;
  if (grados_ini >= 360.0) grados_ini -= 360.0;
  if (grados_ini < 0.0) grados_ini += 360.0;
}

// --- Mostrar datos en pantalla ---
void mostrar_posicion_motor(float angulo) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x13_tr);
  u8g2.drawStr(0, 10, "Posicion Motor:");
  u8g2.setCursor(0, 30);
  u8g2.print(angulo, 1);
  u8g2.print(" ");
  u8g2.print((char)223); // símbolo de grados
  u8g2.setCursor(40, 30);
  u8g2.print(steps, 1);
  u8g2.sendBuffer();
}

// --- Loop principal ---
void loop(void) {
  int xValue = analogRead(VRX_PIN);
  int yValue = analogRead(VRY_PIN);

  // Control con el joystick
  if (abs(xValue - yValue) >= 400) {
    if ((xValue > yValue) && (xValue > 3400)) {
      moveDegrees(dirPin, stepPin, 40.0);   // sentido horario
    } else if ((xValue < yValue) && (xValue < 2700)) {
      moveDegrees(dirPin, stepPin, -40.0);  // sentido antihorario
    }
  }

  mostrar_posicion_motor(grados_ini);
}
