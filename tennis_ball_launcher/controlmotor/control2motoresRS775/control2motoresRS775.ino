#include <U8g2lib.h>
#include <Wire.h>
#include <Arduino.h>

// ===== Pantalla OLED =====
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// ===== Pines del joystick =====
const int VRX_PIN = 35;  
const int VRY_PIN = 32;  
const int SW_PIN  = 33;  

// ===== PWM motores =====
const int pin_motor1 = 25;  
const int pin_motor2 = 26;  
const int frec = 20000;      
const int resolucion = 8;    
const int PWM_MAX = 255;    

// ===== PWM mínimo para arrancar cada motor =====
const float PWM_MIN_MOTOR1 = 11.0f;  
const float PWM_MIN_MOTOR2 = 17.0f;  

// ===== Velocidad motores =====
float vel_motor1 = 0.0f;  
float vel_motor2 = 0.0f;  
const float PASO = 1.0f;    
const int T_RAMPA_MAX = 30000; 

int estado_vertical = 0;     
int estado_horizontal = 0;   

// ==================== FUNCIONES ====================

// Ajuste PWM con compensación mínima
void setPWM(int pin, float porcentaje, float pwm_min){
  if (porcentaje < 0) porcentaje = 0;
  if (porcentaje > 90) porcentaje = 90;

  float salida;

  if (porcentaje == 0){
    salida = 0; // motor parado
  } else {
    salida = pwm_min + (porcentaje / 90.0f) * (90.0f - pwm_min);
  }

  int duty = (int)(salida / 100.0f * PWM_MAX);
  ledcWrite(pin, duty);

  // Actualiza velocidad actual
  if (pin == pin_motor1) vel_motor1 = porcentaje;
  else vel_motor2 = porcentaje;
}

// Mostrar velocidades en OLED
void mostrar_vel_motores(float vel1, float vel2){
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);

  u8g2.setFont(u8g2_font_6x13_tr);
  u8g2.drawStr(10, 14, "Motor1 (%)");
  u8g2.drawStr(80, 14, "Motor2 (%)");

  u8g2.setFont(u8g2_font_profont22_tr);
  u8g2.setCursor(20, 45); u8g2.print((int)vel1);
  u8g2.setCursor(90, 45); u8g2.print((int)vel2);

  u8g2.sendBuffer();
}

// Rampa S-curve para dos motores
void rampaMotores(float velIni1, float velFin1, 
                  float velIni2, float velFin2){
  float delta1 = fabs(velFin1 - velIni1);
  float delta2 = fabs(velFin2 - velIni2);
  float deltaMax = max(delta1, delta2);

  if (deltaMax < 0.1f) { 
    setPWM(pin_motor1, velFin1, PWM_MIN_MOTOR1);
    setPWM(pin_motor2, velFin2, PWM_MIN_MOTOR2);
    mostrar_vel_motores(vel_motor1, vel_motor2);
    return;
  }

  int t_rampa = (int)(T_RAMPA_MAX * (deltaMax / 90.0f));
  if (t_rampa < 50) t_rampa = 50;

  int pasos = max(20, (int)(deltaMax * 2.5f));
  float dt = (float)t_rampa / pasos;

  for (int i = 0; i <= pasos; i++){
    float x = (float)i / pasos;
    float s = 0.5f * (1.0f - cosf(PI * x));

    float vel1 = velIni1 + (velFin1 - velIni1) * s;
    float vel2 = velIni2 + (velFin2 - velIni2) * s;

    setPWM(pin_motor1, vel1, PWM_MIN_MOTOR1);
    setPWM(pin_motor2, vel2, PWM_MIN_MOTOR2);

    mostrar_vel_motores(vel_motor1, vel_motor2);
    delay((int)dt);
  }
}

// ==================== SETUP ====================
void setup(){
  Serial.begin(115200);

  // Configuración PWM
  ledcAttach(pin_motor1, frec, resolucion);
  ledcAttach(pin_motor2, frec, resolucion);

  setPWM(pin_motor1, 0, PWM_MIN_MOTOR1);
  setPWM(pin_motor2, 0, PWM_MIN_MOTOR2);

  // Joystick
  pinMode(SW_PIN, INPUT_PULLUP);
  pinMode(VRX_PIN, INPUT);
  pinMode(VRY_PIN, INPUT);

  // Pantalla
  Wire.begin(21, 22);
  u8g2.begin();
  mostrar_vel_motores(0,0);
}

// ==================== LOOP ====================
void loop(){
  int xValue = analogRead(VRX_PIN);
  int yValue = analogRead(VRY_PIN);
  int swValue = digitalRead(SW_PIN);

  // ===== Botón → parar ambos motores =====
  if (swValue == LOW){
    rampaMotores(vel_motor1, 0.0f, vel_motor2, 0.0f);
    delay(200); 
    return;
  }

  // ===== Lógica de joystick =====
  if(abs(xValue-yValue)>=400){
    if((xValue>yValue) && (xValue>3400)) estado_vertical=-1;
    else if((yValue>xValue) && (yValue>3400)) estado_horizontal=-1;
    else if((xValue<yValue) && (xValue<2700)) estado_vertical=1;
    else if((yValue<xValue) && (yValue<2700)) estado_horizontal=1;
  }
  else{
    estado_vertical=0;
    estado_horizontal=0;
  }

  // ===== Control de velocidad de motores =====
  if (estado_vertical == 1){
    float v1 = min(90.0f, vel_motor1 + PASO);
    float v2 = min(90.0f, vel_motor2 + PASO);
    rampaMotores(vel_motor1, v1, vel_motor2, v2);
  } 
  else if (estado_vertical == -1){
    float v1 = max(0.0f, vel_motor1 - PASO);
    float v2 = max(0.0f, vel_motor2 - PASO);
    rampaMotores(vel_motor1, v1, vel_motor2, v2);
  }

  delay(200);
}
