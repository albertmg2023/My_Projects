//para pantalla oled
#include <U8g2lib.h>
//para i2c
#include <Wire.h>
//para pwm
#include <Arduino.h>

// Inicialización para SSD1306 usando I²C
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE); 

// Pines del joystick
const int VRX_PIN = 35;  // Eje X conectado a ADC1_CH6
const int VRY_PIN = 32;  // Eje Y conectado a ADC1_CH7
const int SW_PIN = 33;   // Botón conectado al pin GPIO14

//pines control motor 1 (dos direcciones)

const int pin_direc1=25;
const int canal_direc1=0;
const int frec1=5000;//frecuencia en Hz pwm hasta 40Mhz
const int resolucion1=8; //bits de la resolución,esta resolucion determina el valor de ciclo 8 bits= 0 a 2^8=255

const int pin_direc2=26;
const int canal_direc2=1;
const int frec2=5000;//frecuencia en Hz pwm hasta 40Mhz
const int resolucion2=8; //bits de la resolución esta resolucion determina el valor de ciclo 8 bits= 0 a 2^8=255

void mostrar_vel_motor(int vel_motor_abs, String direc){

  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);

  u8g2.setFont(u8g2_font_6x13_tr);
  u8g2.drawStr(24, 14, "Vel motor (%)");

  if(direc=="L"){
  
    u8g2.setFont(u8g2_font_profont22_tr);
    u8g2.drawStr(105, 43, "L");

  }
  else if(direc=="R"){
  
    u8g2.setFont(u8g2_font_profont22_tr);
    u8g2.drawStr(105, 43, "R");

  }

  
  
  u8g2.setFont(u8g2_font_profont22_tr);
  u8g2.drawStr(43, 45, String(vel_motor_abs).c_str());
  u8g2.drawEllipse(62, 38, 29, 14);
  delay(100);
  u8g2.sendBuffer();
  



}

void setup(void) {

  //inicializar pwms y asignarles un pin
  //pw1=direc1 especificar características pwm
  //asigno resolución del pin a 8 bits
  analogWriteResolution(pin_direc1, 8);
  //añadir pwm a un pin ,se le pasa el canal especificado anteriormente y un pin
  analogWriteFrequency(pin_direc1, 10000); // set frequency to 10 kHz for LED pin
  
  analogWriteResolution(pin_direc2, 8);
  //añadir pwm a un pin ,se le pasa el canal especificado anteriormente y un pin
  analogWriteFrequency(pin_direc2, 5000);

  



  // Configuración de pines pantalla y joystick
  pinMode(SW_PIN, INPUT_PULLUP);  // Botón con resistencia interna pull-up
  pinMode(VRX_PIN, INPUT);
  pinMode(VRY_PIN, INPUT);

  // Cambiar los pines I²C si no usas los predeterminados
  Wire.begin(21, 22); // SDA = 21, SCL = 22
  u8g2.begin();       // Inicializamos la pantalla
}

// -1 es bajar(x=4095), 1 subir(x=0) y mismositio=0(aprox 2900-3100)

int estado_vertical=0;

// -1 es izq(y=4095), 1 derecha(Y=0) y mismositio=0 (aprox 2900-3100)
int estado_horizontal=0;

int vel_motor=0;
int vel_motor_abs=0;
void loop(void) {

    // Leer valores del joystick
    int xValue = analogRead(VRX_PIN);  // Leer eje X
    int yValue = analogRead(VRY_PIN);  // Leer eje Y
    int swValue = digitalRead(SW_PIN); // Leer botón

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
    }
    //subir vel motor
    if(estado_vertical==1){
      if(vel_motor<=90){
        vel_motor+=10;
      }
    }
    //bajar vel_motor
    else if(estado_vertical==-1){
      if(vel_motor>=-90){
        vel_motor=vel_motor-10;
      }
    }
    //si es negativo gira al otro lado -100

    //vel 100 corresponde a valor de ciclo 255, valorciclo=int (valor%*255)/100 
    //gira derecha
    if(vel_motor<0){
      //se desactiva el pwm de una direccion para que no gire en esa direccion 
      analogWrite(pin_direc2, 0);
      vel_motor_abs=vel_motor*-1;
      analogWrite(pin_direc1,(int)((vel_motor_abs*255)/100));
      mostrar_vel_motor(vel_motor_abs,"R");
      
    }
    //gira izquierda
    else if(vel_motor>0){
      //se desactiva el pwm de una direccion para que no gire en esa direccion 
      analogWrite(pin_direc1, 0);
      vel_motor_abs=vel_motor;
      analogWrite(pin_direc2, (int)((vel_motor_abs*255)/100));
      mostrar_vel_motor(vel_motor_abs,"L");
      
    }
    //paramos los pwm 
    else if(vel_motor==0){
      analogWrite(pin_direc1, 0);
      analogWrite(pin_direc2, 0);
      mostrar_vel_motor(vel_motor,"N");
      
      

    }
    //dibujar velocidad en pantalla y su direccion


    }
    //delay(400)