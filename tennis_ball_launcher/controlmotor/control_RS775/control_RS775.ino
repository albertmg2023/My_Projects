#include <U8g2lib.h>
#include <Wire.h>
#include <Arduino.h>

// ===== Pantalla OLED =====
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// Pines del joystick
const int VRX_PIN = 35;  // Eje X conectado a ADC1_CH6
const int VRY_PIN = 32;  // Eje Y conectado a ADC1_CH7
const int SW_PIN = 33;   // Botón conectado al pin GPIO14

// ===== PWM motor =====
const int pin_direc1 = 25;  // RPWM del BTS7960
const int frec1 = 20000;    // 20 kHz
const int resolucion1 = 8;  // 8 bits → 0..255
const int PWM_MAX = 255;    // máxima salida PWM

// ===== Velocidad motor =====
float vel_motor = 0.0f;      // velocidad actual (%)
const float PASO = 10.0f;    // incremento/decremento
const int T_RAMPA_MAX = 3000; // tiempo máximo 0→90% (ms)

int estado_vertical = 0;     // -1 bajar, 0 reposo, 1 subir
int estado_horizontal = 0;
// ==================== FUNCIONES ====================

void setPWM(float porcentaje){
  if (porcentaje < 0) porcentaje = 0;
  if (porcentaje > 90) porcentaje = 90; 
  int duty = (int)(porcentaje / 100.0f * PWM_MAX);
  ledcWrite(pin_direc1, duty); // función correcta según documentación ESP32
  vel_motor = porcentaje;
}

void mostrar_vel_motor(float vel){
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);

  u8g2.setFont(u8g2_font_6x13_tr);
  u8g2.drawStr(24, 14, "Vel motor (%)");

  u8g2.setFont(u8g2_font_profont22_tr);
  String texto = String((int)vel);
  u8g2.drawStr(50, 45, texto.c_str());
  u8g2.drawEllipse(62, 38, 29, 14);

  u8g2.sendBuffer();
}

// Rampa S-curve proporcional al cambio de velocidad
void rampaSCurve(float velIni, float velFin){
  float delta = fabs(velFin - velIni);
  if (delta < 0.1f) { // cambio muy pequeño
    setPWM(velFin);
    mostrar_vel_motor(velFin);
    return;
  }

  // Tiempo proporcional al cambio
  int t_rampa = (int)(T_RAMPA_MAX * (delta / 90.0f));
  if (t_rampa < 50) t_rampa = 50; // mínimo 50 ms

  // Número de pasos proporcional
  int pasos = max(20, (int)(delta * 2.5f)); // más delta → más pasos
  float dt = (float)t_rampa / pasos;

  for (int i = 0; i <= pasos; i++){
    float x = (float)i / pasos;
    float s = 0.5f * (1.0f - cosf(PI * x)); // S-curve coseno
    float vel = velIni + (velFin - velIni) * s;
    setPWM(vel);
    mostrar_vel_motor(vel);
    delay((int)dt);
  }
}


// ==================== SETUP ====================
void setup(){
  Serial.begin(115200);
  int estado_vertical=0;
  int estado_horizontal=0;
  // Configura PWM en pin con frecuencia y resolución
  ledcAttach(pin_direc1, frec1, resolucion1);
  setPWM(0); // motor parado

  // Joystick
  pinMode(SW_PIN, INPUT_PULLUP);
  pinMode(VRX_PIN, INPUT);

  // Pantalla
  Wire.begin(21, 22); // SDA=21, SCL=22
  u8g2.begin();
  mostrar_vel_motor(0);
}

// ==================== LOOP ====================
void loop(){
  // Leer valores del joystick
    int xValue = analogRead(VRX_PIN);  // Leer eje X
    int yValue = analogRead(VRY_PIN);  // Leer eje Y
    int swValue = digitalRead(SW_PIN); // Leer botón
    if (swValue == LOW){
    rampaSCurve(vel_motor, 0.0f);
    
    }
    else{
        //estados arriba/abajo izq/der 
      if(abs(xValue-yValue)>=400){
        if((xValue>yValue) && (xValue>3400)){
          //bajar
          estado_vertical=-1;
        }
        else if((yValue>xValue) && (yValue>3400)) {

          estado_horizontal=-1;

        }
        else if((xValue<yValue) && (xValue<2700)){
          
          estado_vertical=1;
          
        }
        else if((yValue<xValue) && (yValue<2700)) {

          estado_horizontal=1;

        }
      }
      else{
        estado_vertical=0;
        estado_horizontal=0;
        // Leer valores del joystick
      }
    
    // Subir velocidad
    if (estado_vertical == 1){
      float velNueva = vel_motor + PASO;
      if (velNueva > 90) velNueva = 90;
      rampaSCurve(vel_motor, velNueva);
    }
    // Bajar velocidad
    else if (estado_vertical == -1){
      float velNueva = vel_motor - PASO;
      if (velNueva < 0) velNueva = 0;
      rampaSCurve(vel_motor, velNueva);
    }
  }
  delay(200); // evita repeticiones rápidas
}
