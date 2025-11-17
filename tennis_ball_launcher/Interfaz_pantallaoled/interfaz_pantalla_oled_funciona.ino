#include <U8g2lib.h>
#include <Wire.h>
#include <Preferences.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
//inicializamos uso memoria flash(la que guarda en almacenamiento)
Preferences memoria;
// Inicialización para SSD1306 usando I²C
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE); 

//PARA INTERFAZ GRÁFICA CON OLED 128X64

//cañon
  static const unsigned char cañon[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x38,0xf0,0x0f,0x00,0x00,0x00,0xf8,0x0e,0x70,0x3f,0x00,0x00,0x07,0x00,0x00,0x2e,0x00,0x80,0x01,0x00,0x00,0x5a,0x00,0x40,0x01,0x00,0x00,0x7e,0x00,0xc0,0x00,0x00,0x00,0x7a,0x00,0xf0,0x00,0x00,0x00,0x7e,0x00,0x48,0x00,0x00,0x00,0x7a,0x00,0x48,0x00,0x00,0x00,0x7c,0x00,0x44,0x00,0x0f,0x00,0x6c,0x00,0x4a,0xf0,0x1c,0x00,0x2d,0x00,0xca,0x08,0x1c,0x40,0x3d,0x00,0x0a,0x09,0x38,0x55,0x0d,0x00,0x1a,0x09,0x78,0x55,0x03,0x00,0x34,0x0b,0xb8,0xea,0x01,0x00,0x38,0x07,0xa8,0x3a,0x00,0x00,0xe0,0xe6,0xf1,0x0f,0x00,0x00,0x00,0xf9,0xd3,0x02,0x00,0x00,0x80,0xb8,0xe7,0x01,0x00,0x00,0xc0,0xf8,0x67,0x00,0x00,0x00,0x30,0x3c,0x6f,0x00,0x00,0x00,0x0c,0x1c,0x1f,0x00,0x00,0x00,0x03,0x1c,0x0f,0x00,0x00,0xc0,0xe0,0x3f,0x0f,0x00,0x00,0x40,0x1e,0xf8,0x07,0x00,0x00,0x80,0x01,0x70,0x03,0x00,0x00,0x00,0x00,0xf0,0x01,0x00,0x00,0x00,0x00,0xe0,0x01,0x00,0x00};
  //raqueta
  static const unsigned char raqueta[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0x00,0x00,0x00,0x00,0xe0,0xe3,0x03,0x00,0x00,0x00,0xf0,0xbc,0x07,0x00,0x00,0x00,0x38,0xdb,0x0f,0x00,0x00,0x00,0x7c,0x81,0x1f,0x00,0x00,0x00,0xde,0x00,0x16,0x00,0x00,0x00,0x2f,0x00,0x16,0x00,0x00,0x00,0x37,0x00,0x16,0x00,0x00,0x80,0x0b,0x00,0x16,0x00,0x00,0x80,0x0d,0x00,0x16,0x00,0x00,0x80,0x05,0x00,0x17,0x00,0x00,0x80,0x02,0x00,0x17,0x00,0x00,0xc0,0x06,0x80,0x1b,0x00,0x00,0xc0,0x06,0x80,0x1b,0x00,0x00,0xc0,0x06,0xc0,0x1d,0x00,0x00,0xc0,0x02,0xe0,0x0d,0x00,0x00,0xc0,0x05,0xe0,0x0e,0x00,0x00,0xe0,0x1f,0xbe,0x03,0x00,0x00,0xe0,0xff,0xdf,0x01,0x00,0x00,0xe0,0xff,0xe7,0x00,0x00,0x00,0xf0,0xff,0x38,0x00,0x00,0x00,0xf0,0x00,0x1f,0x00,0x00,0x00,0x78,0xff,0x07,0x00,0x00,0x00,0xd4,0xff,0x00,0x00,0x00,0x00,0x61,0x00,0x00,0x00,0x00,0x80,0x30,0x00,0x00,0x00,0x00,0x80,0x18,0x00,0x00,0x00,0x00,0x40,0x0c,0x00,0x00,0x00,0x00,0x20,0x06,0x00,0x00,0x00,0x00,0x10,0x03,0x00,0x00,0x00,0x00,0x88,0x01,0x00,0x00,0x00,0x00,0x84,0x00,0x00,0x00,0x00,0x00,0x68,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  static const unsigned char pelota2[] = {0x38,0x00,0xfc,0x00,0xc6,0x00,0x9e,0x01,0xbf,0x01,0xdf,0x01,0xee,0x01,0xee,0x01,0xdc,0x00,0x38,0x00};
  //imagen pelota1
  static const unsigned char pelota1[] = {0xf8,0x00,0xfe,0x01,0xe6,0x01,0xd3,0x02,0x3b,0x03,0xfb,0x03,0xfe,0x01,0xfc,0x00,0x30,0x00};
  static const unsigned char pelota3[] = {0x30,0x00,0xfc,0x00,0xfe,0x01,0x7f,0x03,0x73,0x03,0x2d,0x03,0x9e,0x01,0xfe,0x01,0x7c,0x00};
  //explosiones

  static const unsigned char explosioncañon[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x80,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x03,0x00,0x40,0xbe,0x03,0x00,0x00,0xbd,0x0f,0x00,0x00,0xff,0x0d,0x00,0x00,0xfe,0x07,0x00,0x00,0xef,0x07,0x00,0x00,0x3d,0x0f,0x00,0x00,0xf7,0x0f,0x00,0x00,0xfe,0x0f,0x00,0x00,0xfe,0x06,0x00,0x00,0x38,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  static const unsigned char explosion1[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x80,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x03,0x00,0x40,0xbe,0x03,0x00,0x00,0xbd,0x0f,0x00,0x00,0xff,0x0d,0x00,0x00,0xfe,0x07,0x00,0x00,0xef,0x07,0x00,0x00,0x3d,0x0f,0x00,0x00,0xf7,0x0f,0x00,0x00,0xfe,0x0f,0x00,0x00,0xfe,0x06,0x00,0x00,0x38,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  static const unsigned char explosion2[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0x00,0x00,0x00,0xc0,0xff,0x01,0x00,0x40,0xeb,0xff,0xff,0x00,0xc0,0xff,0xbf,0xfe,0x00,0xe0,0xff,0x1f,0xfe,0x01,0xe0,0xff,0x3f,0xf9,0x03,0xe0,0xff,0x7f,0xf2,0x01,0xe0,0xff,0xff,0xe0,0x01,0xc0,0x1f,0xfe,0xed,0x00,0xc0,0xa7,0xfe,0xf7,0x01,0xe0,0x0f,0xf8,0xff,0x01,0xe0,0x4b,0xfa,0xff,0x01,0xe0,0x97,0xf0,0xff,0x03,0xe0,0x5f,0xf6,0xff,0x03,0xe0,0x3f,0xf4,0xff,0x03,0xe0,0xff,0xff,0xfb,0x03,0xe0,0xff,0xff,0xf1,0x03,0xe0,0xff,0xff,0xf5,0x01,0xe0,0xff,0xff,0xf5,0x01,0xc0,0xff,0xff,0xf9,0x01,0xc0,0xff,0xfe,0xfb,0x02,0xc0,0x7f,0xf1,0xff,0x03,0xc0,0xff,0xe8,0xff,0x07,0xc0,0xff,0xf3,0x7f,0x07,0xc0,0xff,0xff,0xff,0x07,0xc0,0xe7,0xff,0xff,0x07,0x00,0xdf,0xff,0xdf,0x01,0x00,0x7e,0xd5,0x1f,0x00,0x00,0xfc,0xff,0x0f,0x00,0x00,0xe0,0xff,0x0f,0x00,0x00,0x40,0x2f,0x06,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  static const unsigned char explosion3[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0x01,0x00,0x00,0x00,0x00,0x00,0x80,0xff,0x1f,0x00,0x00,0x00,0x00,0x54,0xc1,0xff,0xbf,0xa8,0x02,0x00,0x00,0xfc,0xe3,0xff,0xff,0xf9,0x03,0x00,0x00,0xfe,0xff,0xff,0xff,0xff,0x0f,0x00,0x80,0xff,0xff,0xff,0xff,0xff,0x7f,0x00,0xc0,0x1f,0xfe,0xff,0xff,0xff,0xff,0x00,0xc0,0x8f,0xff,0xff,0xff,0x07,0xfe,0x01,0xc0,0xef,0xff,0xff,0xff,0x27,0xfc,0x01,0xe0,0xf7,0xff,0xe7,0xff,0x0f,0xf8,0x01,0xe0,0xff,0xff,0xc3,0xc3,0x1f,0xf9,0x01,0xe0,0xff,0x6f,0xc9,0xc3,0x3f,0xfd,0x01,0xe0,0xff,0x03,0xc8,0x01,0x3e,0xfc,0x01,0xf0,0xff,0xe3,0xc0,0x43,0x3c,0xfc,0x00,0xf0,0xff,0x01,0x00,0x0e,0x38,0xfc,0x00,0xf0,0xff,0xc1,0xc0,0x06,0xf0,0x3e,0x00,0xf0,0xff,0x00,0x00,0x03,0xf0,0x3f,0x00,0xf0,0xff,0x02,0x00,0x08,0xe4,0x1f,0x00,0xe0,0x3f,0x00,0x00,0x00,0xf0,0x3f,0x00,0xe0,0xbf,0x01,0x00,0x20,0xf0,0x3f,0x00,0xe0,0xff,0x01,0x00,0x10,0xf2,0x7f,0x00,0xe0,0xff,0x01,0x00,0x10,0xff,0xff,0x00,0xc0,0xbf,0x03,0x80,0x81,0xff,0xff,0x00,0xc0,0x3f,0x00,0x00,0xe0,0xff,0xff,0x00,0xc0,0xff,0x00,0x00,0xf8,0xff,0xff,0x00,0xc0,0xff,0x08,0x01,0xff,0xff,0xff,0x00,0xe0,0xff,0x63,0x00,0xff,0x07,0xfc,0x00,0xe0,0xff,0xff,0xff,0xff,0x01,0xfc,0x00,0xe0,0xff,0xdf,0xff,0xff,0x00,0xfe,0x00,0xe0,0xff,0x8f,0xff,0xff,0x00,0xfe,0x00,0xe0,0xff,0xff,0xff,0xff,0x00,0x3e,0x00,0xe0,0xff,0xff,0xff,0x3f,0x00,0x7e,0x00,0xe0,0x87,0xff,0xff,0x3f,0x10,0x1f,0x00,0xe0,0x27,0x80,0xff,0x3f,0x00,0x1f,0x00,0xe0,0xa7,0xe0,0xff,0x7f,0x00,0x3f,0x00,0xe0,0x4f,0xe8,0x1f,0xff,0x50,0x7f,0x00,0xe0,0x0f,0xe8,0x07,0xfe,0xd7,0xff,0x00,0xc0,0x0f,0xe0,0x11,0xf8,0xc7,0xff,0x00,0x80,0xff,0xf3,0x03,0xf8,0xff,0xff,0x01,0x00,0xff,0xff,0x07,0xf8,0xff,0xfd,0x01,0xc0,0xff,0xff,0x03,0xe0,0xff,0xff,0x01,0xe0,0xff,0xff,0x01,0x88,0xff,0xff,0x03,0xe0,0x8f,0xf7,0x40,0x00,0xff,0xcf,0x03,0xe0,0xff,0x7f,0x03,0x40,0xfa,0xe7,0x03,0xe0,0xff,0x7f,0x12,0x48,0xfe,0xf3,0x03,0xe0,0x1f,0x7f,0x12,0x08,0xff,0xf1,0x03,0xc0,0x1f,0xfe,0x01,0xa0,0xff,0xf9,0x03,0xc0,0x1f,0xfc,0x1f,0x94,0xff,0xff,0x01,0x80,0xff,0xf0,0xff,0xc0,0xff,0xff,0x00,0x00,0xff,0xc1,0xff,0xff,0xff,0x7f,0x00,0x00,0xfe,0x83,0xff,0xff,0xff,0x1f,0x00,0x00,0xfe,0x8f,0xff,0xff,0xc7,0x07,0x00,0x00,0xfa,0xff,0xff,0xff,0x47,0x05,0x00,0x00,0xf8,0xff,0xff,0xff,0x03,0x00,0x00,0x00,0xe0,0xff,0x9f,0x7f,0x00,0x00,0x00,0x00,0x00,0xfc,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  

//flecha que apunta a boton atras
static const unsigned char image_Layer_5_bits[] = {0x20,0x00,0x40,0x00,0x80,0x00,0xff,0x01,0x80,0x00,0x40,0x00,0x20,0x00};

//flecha de posicionamiento meno posivcion robot
static const unsigned char image_Layer_8_bits[] = {0x41,0x22,0x14,0x08,0x14,0x22,0x41};

//menu principal figuras
//localizacion y mando
static const unsigned char image_download_bits[] = {0x7c,0x00,0x00,0x82,0x00,0x00,0xba,0x00,0x00,0xba,0x00,0x00,0x92,0x00,0x00,0x44,0x00,0x00,0x44,0x00,0x00,0x28,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0x03,0x00,0xfb,0x07,0x00,0x71,0x05,0x00,0xfb,0x07,0x00,0x8f,0x07,0x00,0x07,0x07,0x00,0x03,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10};
static const unsigned char image_Layer_17_bits[] = {0x40,0x20,0x00,0x20,0x70,0x20,0x10,0xa8,0x40,0x28,0x20,0xbe,0x44,0x20,0x40,0x42,0x20,0x20,0x21,0x00,0x00,0xff,0x01,0x00};
static const unsigned char image_zjvjt57_pesaspesas_bits[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x3c,0xf0,0x00,0x3c,0xf0,0x00,0xff,0xff,0x01,0xff,0xff,0x01,0x3c,0xf0,0x00,0x3c,0xf0,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//pines control motor 

// ===== PWM motores =====
const int pin_motor1 = 25;  
const int pin_motor2 = 26;
const int pin_motor3 = 27;  
const int frec = 20000;      
const int resolucion = 8;    
const int PWM_MAX = 255;    

// ===== PWM mínimo para arrancar cada motor =====
const float PWM_MIN_MOTOR1 = 11.0f;  
const float PWM_MIN_MOTOR2 = 17.0f; 
const float PWM_MIN_MOTOR3 = 0.0f; 

// ===== Velocidad motores =====
float vel_motor3 = 0.0f;
float vel_motor1 = 0.0f;  
float vel_motor2 = 0.0f;  
const float PASO = 1.0f;    
const int T_RAMPA_MAX = 3000; 


// Pines del joystick
const int VRX_PIN = 35;  // Eje X conectado a ADC1_CH6
const int VRY_PIN = 32;  // Eje Y conectado a ADC1_CH7
const int SW_PIN = 33;   // Botón conectado al pin GPIO14

//pin LDR y configuracion detector
const int LDR_PIN = 34;           // Pin del fotoresistor
const int UMBRAL_OSCURIDAD = 1000; // Valor bajo indica oscuridad
const unsigned long INTERVALO_LECTURA = 200;  // Leer cada 200 ms

//pines control motor 1 (dos direcciones)



//parado del motor

bool stopMotor=true;

// estado global[pulsado,estadoX,estadoY]
int estado_joystick[3]={0,0,0};

//Estado menú

String estado_menu="Inicio";


//datos Modo_manual

int datos_manual[3]={0};

//posicion del robot
float pos_robot_real[2]={0};

// -1 es bajar(x=4095), 1 subir(x=0) y mismositio=0(aprox 2900-3100)

int estado_vertical=0;

// -1 es izq(y=4095), 1 derecha(Y=0) y mismositio=0 (aprox 2900-3100)
int estado_horizontal=0;

// Variable global que avisa si se han completado las repeticiones
bool repeticiones_completadas = false;
// Contador de pelotas
volatile int contador_pelotas = 0;
//mutex para la variable contador pelotas
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
unsigned long ultimoTiempoLectura = 0; // Variable para controlar el intervalo de lectura
int num_entrenamiento=0;
int contanim=0;

// Pines de los botones(final de carrera)
const int BOTON_VA = 12;  // Bloquea sentido horario del angulo Veritcal
const int BOTON_VB = 13;  // Bloquea sentido antihorario del angulo Veritcal
const int BOTON_HA = 15;  // Bloquea sentido horario del angulo Horizontal
const int BOTON_HB = 17;  // Bloquea sentido antihorario del angulo Horizontal

// Pines del motorV
const int dirPinV  = 23;
const int stepPinV = 14;
// Pines del motorH
const int dirPinH  = 4;
const int stepPinH = 16;

// Variables de control

//angulos del lanzador
float grados_ini_V = 0.0;
float grados_ini_H = 0.0;
const int stepsPerRev = 200;     // Número de pasos por revolución del motor en modo full-step
const unsigned int pulseUs = 400; // Ancho de pulso seguro para DRV8825
// Variables de interrupción
volatile bool detenerGiroHorarioV = false;
volatile bool detenerGiroAntihorarioV = false;
volatile bool detenerGiroHorarioH = false;
volatile bool detenerGiroAntihorarioH = false;

//Interrupciones para cuando se llega a los finales de carrera
// --- Interrupciones ---
void IRAM_ATTR interrupcionBotonAV() {
  detenerGiroHorarioV = (digitalRead(BOTON_VA) == LOW);  // LOW = pulsado (por INPUT_PULLUP)
}

void IRAM_ATTR interrupcionBotonBV() {
  detenerGiroAntihorarioV = (digitalRead(BOTON_VB) == LOW);
}
void IRAM_ATTR interrupcionBotonAH() {
  detenerGiroHorarioH = (digitalRead(BOTON_HA) == LOW);  // LOW = pulsado (por INPUT_PULLUP)
}

void IRAM_ATTR interrupcionBotonBH() {
  detenerGiroAntihorarioH = (digitalRead(BOTON_HB) == LOW);
}



//FUNCIONES PARA GUARDAR EN MEMORIA LOS ENTRENAMIENTOS Y SACARLOS DESDE LA MEMORIA

// Array bidimensional en vez de lista de listas,se debe definir una array bidimensional de 20filas=numejercicios, 5 col=datos de cada ejercicio
#define MAX_FILAS 20
#define MAX_COLUMNAS 5

void guardarMatriz(int (*ent)[5], int filas, int columnas, String lugar) {
    memoria.begin("storage", false);

    String data = "";
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            data += String(ent[i][j]); 
            if (j < columnas - 1) data += ","; 
        }
        if (i < filas - 1) data += ";"; 
    }

    memoria.putString(lugar.c_str(), data);
    Serial.println("Matriz guardada: " + data);
    
    memoria.end();
}

void contarFilasColumnas(String data, int &filas, int &columnas) {
    filas = 1; 
    columnas = 1;

    if (data.length() == 0) {
        filas = 0;
        columnas = 0;
        return;
    }

    for (char c : data) {
        if (c == ';') filas++;
    }

    columnas = 1;
    for (char c : data) {
        if (c == ',') columnas++;
        if (c == ';') break;
    }
}

void recuperarLista(int listaDeListas[MAX_FILAS][MAX_COLUMNAS], String lugar) {
    memoria.begin("storage", true);
    String data = memoria.getString(lugar.c_str(), ""); 
    memoria.end();

    if (data == "") {
        Serial.println("No hay datos en memoria.");
        return;
    }

    int filas, columnas;
    contarFilasColumnas(data, filas, columnas);

    Serial.print("Filas detectadas: ");
    Serial.print(filas);
    Serial.print(", Columnas detectadas: ");
    Serial.println(columnas);
    
    int fila = 0;
    char buffer[data.length() + 1];
    data.toCharArray(buffer, data.length() + 1);

    char *saveptrFila;
    char *saveptrColumna;
    char *tokenFila = strtok_r(buffer, ";", &saveptrFila);

    while (tokenFila != NULL && fila < filas) {
        int columna = 0;
        char *tokenColumna = strtok_r(tokenFila, ",", &saveptrColumna);
        while (tokenColumna != NULL && columna < columnas) {
            listaDeListas[fila][columna] = atoi(tokenColumna);
            tokenColumna = strtok_r(NULL, ",", &saveptrColumna);
            columna++;
        }
        tokenFila = strtok_r(NULL, ";", &saveptrFila);
        fila++;
    }

    Serial.println("Lista reconstruida:");
    for (int i = 0; i < filas; i++) {
        Serial.print("Fila ");
        Serial.print(i);
        Serial.print(": ");
        for (int j = 0; j < columnas; j++) {
            Serial.print(listaDeListas[i][j]);
            Serial.print(" ");
        }
        Serial.println();
    }
}



//Funciones para mostrar menu en la pantalla oled
void update_joystick(){
    
   // Leer valores del joystick
    int xValue = analogRead(VRX_PIN);  // Leer eje X
    int yValue = analogRead(VRY_PIN);  // Leer eje Y
    int swValue = digitalRead(SW_PIN); // Leer botón
    delay(100);
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
        //bajar
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

    //cambia el estado global del joystick
    estado_joystick[0]=swValue;
    estado_joystick[1]=estado_horizontal;
    estado_joystick[2]=estado_vertical;


}
void mostrar_anim_inicio(){


  for(int i=0;i<2;i++){

    u8g2.clearBuffer();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);

    u8g2.drawXBM(-5, 17, 48, 35, cañon);
  
    u8g2.drawXBM(82, 13, 42, 40, raqueta);
    u8g2.sendBuffer();
    delay(400);
    u8g2.clearBuffer();
    u8g2.drawXBM(-5, 17, 48, 35, cañon);
    u8g2.drawXBM(82, 13, 42, 40, raqueta);
    //explosion cañon
    u8g2.drawXBM(27, 16, 30, 27, explosioncañon);
    u8g2.sendBuffer();
    delay(200);
    u8g2.clearBuffer();
    u8g2.drawXBM(-5, 17, 48, 35, cañon);
    u8g2.drawXBM(82, 13, 42, 40, raqueta);
    u8g2.drawXBM(51, 26, 10, 9, pelota1);
    u8g2.sendBuffer();
    delay(200);
    u8g2.clearBuffer();
    u8g2.drawXBM(-5, 17, 48, 35, cañon);
    u8g2.drawXBM(82, 13, 42, 40, raqueta);
    u8g2.drawXBM(68, 26, 9, 10, pelota2);
    u8g2.sendBuffer();
    delay(200);
    u8g2.clearBuffer();
    u8g2.drawXBM(-5, 17, 48, 35, cañon);
    u8g2.drawXBM(82, 13, 42, 40, raqueta);
    u8g2.drawXBM(87, 26, 10, 9, pelota3);
    u8g2.sendBuffer();
    delay(300);
    u8g2.clearBuffer();
    u8g2.drawXBM(-5, 17, 48, 35, cañon);
    u8g2.drawXBM(82, 13, 42, 40, raqueta);
    u8g2.drawXBM(86, 19, 30, 27, explosion1);
    u8g2.sendBuffer();
    delay(300);
    u8g2.clearBuffer();
    u8g2.drawXBM(-5, 17, 48, 35, cañon);
    u8g2.drawXBM(82, 13, 42, 40, raqueta);
    u8g2.drawXBM(82, 8, 40, 40, explosion2);
    u8g2.sendBuffer();
    delay(300);
    u8g2.clearBuffer();
    u8g2.drawXBM(-5, 17, 48, 35, cañon);
    u8g2.drawXBM(82, 13, 42, 40, raqueta);
    u8g2.drawXBM(69, 2, 62, 62, explosion3);
    delay(300);
    u8g2.sendBuffer();
  }
  }

//posiciones del cursor,de forma global
int fila=0;
int col=0;

void ejecutar_menuprincipal(){
  fila=0;
  
  //bucle hasta que cambia de menú
  bool seguir=true;
  while(seguir){
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.clearBuffer();
    u8g2.drawFrame(4, 2, 120, 13);
    u8g2.drawFrame(4, 16, 120, 13);
    u8g2.drawFrame(4, 30, 120, 13);
    u8g2.setFont(u8g2_font_4x6_tr);
    u8g2.drawUTF8(32, 11, " Localización");
    u8g2.drawStr(34, 24, "Entrenamientos");
    u8g2.drawStr(38, 39, "Modo Manual");
    u8g2.drawXBM(11, 4, 21, 43, image_download_bits);
    u8g2.drawXBM(7, 10, 17, 23, image_zjvjt57_pesaspesas_bits);
    u8g2.drawFrame(4, 46, 120, 13);
    u8g2.drawUTF8(46, 54, "Angulos");
    u8g2.drawFrame(115, 39, 1, 1);
    u8g2.drawXBM(12, 48, 24, 8, image_Layer_17_bits);

    //actualiza las mediciones del joystick
    update_joystick();
    //miro estado vertical(arriba,abajo o centro) y actuo muestro el selector y hago acciones

    //si no se está pulsando=0 mostrar solo el selector
    if(estado_joystick[0]==1){
      //hacia arriba
      if( estado_joystick[2]==1){
        //si no el elemento de más arriba
        if(fila>0 && fila<=3){

         fila=fila-1;
          
        }
        
      }
      else if( estado_joystick[2]==-1){
        //si no el elemento de más arriba
        if(fila<3 && fila>=0){

         fila=fila+1;
          
        }
        
      }
      if(fila==0){
        //selected 1 localización
        u8g2.drawBox(108, 5, 9, 7);
  
      }
      else if(fila==1){
        //selected 2 entrenamientos
        u8g2.drawBox(108, 19, 9, 7);
      
      }
      else if(fila==2){
        //selected 3 modo manual
        u8g2.drawBox(108, 33, 9, 7);
      
      }
      else if(fila==3){
        //selected 4 angulos
        u8g2.drawBox(108, 49, 9, 7);
      
      }
      u8g2.sendBuffer();
      
    }
    //si se pulsa
    else{
      if(fila==0){

        estado_menu="Localizacion";
        seguir=false; 
      }
      else if(fila==1){
        estado_menu="Entrenam";
        seguir=false;
      }
      else if(fila==2){
        estado_menu="Modo_Manual" ;
        seguir=false;
      }
      else if(fila==3){
        estado_menu="Angulos" ;
        seguir=false;
      } 
      
    } 

  //sale del bucle del menu principal si se selecciona algo
   
  }
  
}
//entrenamientos
  int ent1[20][5] = {0};
  int ent2[20][5] = {0};
  int ent3[20][5] = {0};
void editar_entrenamiento(int num_entrenamiento){
    int fila=0;
    int col=0;
    int (*ent)[5] = nullptr;  // Puntero a un array de 5 elementos

      // Seleccionar la matriz correcta,es lo mismo que el primer vector de la matriz,
      //luego se puede acceder a los elementos de ent de la misma forma que una amtriz normal
      if (num_entrenamiento == 1) {
          ent = ent1;
      } else if (num_entrenamiento == 2) {
          ent = ent2;
      } else if (num_entrenamiento == 3) {
          ent = ent3;
      }

    
    int ejer=0;
    bool seguir3=true;
    //hasta que se da a atras
    String reps= String(ent[ejer][0]);
    String motor_sup=String(ent[ejer][1]);
    String motor_inf=String(ent[ejer][2]);
    String motor_3= String(ent[ejer][3]);
    //String angRL= String(ent[ejer][3]);
    String angUD=String(ent[ejer][4]);
    String nejer="";

    
    while(seguir3){
      reps= "Reps "+String(ent[ejer][0]);
      motor_sup="Motor_sup "+String(ent[ejer][1]);
      motor_inf="Motor_inf "+String(ent[ejer][2]);
      motor_3= "motor_3 "+String(ent[ejer][3]);
      //angRL= "AngRL "+String(ent[ejer][3]);
      angUD="AngUD "+String(ent[ejer][4]);
      nejer=String(ejer)+"/19";
      update_joystick();
      u8g2.clearBuffer();
      u8g2.setFontMode(1);
      u8g2.setBitmapMode(1);
      u8g2.setFont(u8g2_font_5x8_tr);
      //numero ejercicio actual
      u8g2.drawStr(95, 56, nejer.c_str());
      u8g2.setFont(u8g2_font_4x6_tr);
      u8g2.drawStr(6, 13, reps.c_str());
      u8g2.drawStr(6, 23, motor_sup.c_str());
      u8g2.drawStr(6, 32, motor_inf.c_str());
      u8g2.drawStr(6, 42, motor_3.c_str());
      //u8g2.drawStr(6, 42, angRL.c_str());
      u8g2.drawStr(6, 53, angUD.c_str());
      //str del boton atras
      u8g2.drawUTF8(104, 8, "Atrás");
      //boton save
      u8g2.drawFrame(92, 19, 30, 20);
      u8g2.setFont(u8g2_font_6x13_tr);
      u8g2.drawStr(95, 33, "Save");
      //cuadrado del boton atras
      u8g2.drawFrame(102, 2, 23, 9);
      

    //si no se pulsa el joystick
      if (estado_joystick[0] == 1) {
        
        //a la izquierda
        if ((estado_joystick[1] == -1) && col>0 ) {
          col--;
          fila=0;
        }
        //a la derecha
        else if ((estado_joystick[1]) == 1 && col<1 ) {
          col++;
          //pasa a boton de cambiar ejer
          fila=2;
        }
        if(col==0){
          //hacia arriba
          if (estado_joystick[2] == 1 && fila > 0 &&  fila<=4){
          fila--;
          } 
          //hacia abajo
          else if (estado_joystick[2] == -1 && fila < 4 && fila>=0) {
            fila++;
          }
        }
        else if(col==1){
          //hacia arriba
          if (estado_joystick[2] == 1 && fila > 0){
          fila--;
          } 
          //hacia abajo
          else if (estado_joystick[2] == -1 && fila < 2 ) {
            fila++;
          }

        }
        if(col==0){
          if(fila==0){
            //cuadrado selct reps
            u8g2.drawBox(72, 8, 5, 5);
          } 
          else if(fila==1){
            //cuadrado selct vel mot sup
            u8g2.drawBox(72, 18, 5, 5);
          }
          else if(fila==2){
            //cuadrado selct vel mot inf
            u8g2.drawBox(66, 27, 5, 5);
          }
          else if(fila==3){
          //cuadrado selct AnguloR/L
            u8g2.drawBox(72, 37, 5, 5);
          }
          else if(fila==4){
            //cuadrado selct anguloU_D
            u8g2.drawBox(72, 48, 5, 5);
          }
          
        }
        else if(col==1){
          if(fila==0){
            //dibujar flecha atras
            u8g2.drawXBM(89, 3, 9, 7, image_Layer_5_bits);
            
          }
          else if(fila==1){
            //boton save selector
            u8g2.drawLine(91, 17, 89, 17);
            u8g2.drawLine(89, 18, 89, 41);
            u8g2.drawLine(90, 41, 91, 41);
            u8g2.drawLine(124, 17, 124, 40);
            u8g2.drawLine(124, 41, 122, 41);
            u8g2.drawLine(122, 17, 124, 17);

          }
          else if(fila==2){
            u8g2.drawLine(116, 47, 116, 56);

            u8g2.drawLine(93, 48, 93, 57);

            u8g2.drawLine(93, 58, 95, 58);

            u8g2.drawLine(93, 47, 95, 47);

            u8g2.drawLine(114, 47, 116, 47);

            u8g2.drawLine(114, 58, 116, 58);
          }

        }
      }
      //cuando se pulsa 
      else if(estado_joystick[0] == 0){
        if(col<=0){
          if (fila == 0){
            cambiar_valores_ejer_mod_entren(ent,ejer,0,1,19,0);
          
          } 
          else if (fila == 1){
            
            cambiar_valores_ejer_mod_entren(ent,ejer,1,1,100,0);
          }
          else if (fila == 2){
            cambiar_valores_ejer_mod_entren(ent,ejer,2,1,100,0);
          }
          else if (fila == 3){
            cambiar_valores_ejer_mod_entren(ent,ejer,3,1,90,-90);
          }
          else if (fila == 4){
            cambiar_valores_ejer_mod_entren(ent,ejer,4,1,90,-90);
          }
          
          
        }
        else if (col>=1){
          if(fila==0){
          seguir3=false;
          estado_menu="Entrenam"; // Si se pulsa el botón del joystick
          }
          else if(fila==1){
          // Ahora que el puntero 'ent' apunta a la matriz correcta, podemos guardarla en memoria.
          guardarMatriz(ent, 20, 5, "entrenamiento" + String(num_entrenamiento));
          delay(300);

          
          }
          //se ha pulsado para guardar en memoria fila 1
          
          else if(fila==2){
          //bucle de cambio de valores de entrenamiento
          ejec_cambiar_ejer_mod_entren(ent,ejer,1,19,0);

          
          }
        }
        
        
      }
    u8g2.sendBuffer();
  }
}
void ejecutar_menu_entrenamientos() {
  bool seguir1 = true;
  int fila = 0;
  int col = 0;
  update_joystick();

  while (seguir1) {
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.clearBuffer();

    // Dibujar el marco y texto del menú
    u8g2.drawFrame(3, 32, 120, 11);
    u8g2.drawFrame(3, 17, 120, 11);
    u8g2.drawFrame(3, 47, 120, 11);
    u8g2.setFont(u8g2_font_4x6_tr);
    u8g2.drawStr(34, 25, "Entrenamiento 1");
    u8g2.drawStr(34, 40, "Entrenamiento 2");
    u8g2.drawStr(34, 55, "Entrenamiento 3");
    //dibuja atras
    u8g2.drawFrame(102, 1, 23, 9);
    u8g2.setFont(u8g2_font_4x6_tr);
    u8g2.drawUTF8(104, 8, "Atrás");
    update_joystick(); // Solo una llamada por iteración

    // Control del joystick
    if (estado_joystick[0] == 1) {
      if (estado_joystick[2] == 1 && fila > 0 && col==0 and fila<=2) {
        fila--;
      } 
      else if (estado_joystick[2] == -1 && fila < 2 && col==0 and fila>=0) {
        fila++;
      }
      else if (estado_joystick[1] == -1 && col > 0) {
        col--;
        fila=0;
      }
      else if (estado_joystick[1] == 1 && col < 1) {
        col++;
        fila=0;
      }
      //columna de entrenaminetos
      if(col<=0){
        if(fila<=0){
          u8g2.drawBox(107, 20, 6, 5);
        }
        else if(fila==1){
          u8g2.drawBox(107, 36, 6, 5);
        }
        else if(fila>=2){
          u8g2.drawBox(107, 50, 6, 5);
        }

      }
      //columna atras
      else{

        //dibujar flecha atras
        u8g2.drawXBM(89, 3, 9, 7, image_Layer_5_bits);


      }

      
    } 
    else if(estado_joystick[0] == 0){
      if(col<=0){
        if (fila <= 0) {
        estado_menu = "ENT1";
        num_entrenamiento=1;
        }
        else if (fila == 1) {
        estado_menu = "ENT2";
        num_entrenamiento=2;
        }
        else if (fila >= 2) {
        estado_menu = "ENT3";
        num_entrenamiento=3;
        }
        seguir1=false; // Si se pulsa el botón del joystick
      
      

      }
      else if (col>=1){
        estado_menu="Inicio";
        seguir1=false; // Si se pulsa el botón del joystick

      }
      
      
    }
    // Enviar buffer SIEMPRE
    u8g2.sendBuffer();
  }
}

void ejecutar_entrenamiento(int n_entren){

  String opcion="";
  mostrar_opciones_entranamiento(opcion);
  
  if(opcion=="Editar"){

    editar_entrenamiento(n_entren);

  }
  
  else if(opcion=="Atras"){
    estado_menu="Entrenam";

  }
  
}

void mostrar_opciones_entranamiento(String &opcion){
  bool seguir2 = true;
  int fila = 0;
  int col = 0;
  int (*ent)[5] = nullptr;  // Puntero a un array de 5 elementos

      // Seleccionar la matriz correcta,es lo mismo que el primer vector de la matriz,
      //luego se puede acceder a los elementos de ent de la misma forma que una amtriz normal
      if (num_entrenamiento == 1) {
          ent = ent1;
      } else if (num_entrenamiento == 2) {
          ent = ent2;
      } else if (num_entrenamiento == 3) {
          ent = ent3;
      }
  //Copio ent para acceder desde la tarea sin conflictos de lect_escritura
  int copia_ent[20][5];  // Variable local (en stack) donde guardarás la copia
  memcpy(copia_ent, ent, sizeof(copia_ent));
  update_joystick();
  while (seguir2) {
    update_joystick();
    u8g2.clearBuffer();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    
    u8g2.setFont(u8g2_font_5x8_tr);
    u8g2.drawFrame(9, 22, 56, 11);
    u8g2.drawStr(21, 31, "EDITAR");
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(96, 35, "STOP");
    u8g2.drawFrame(91, 20, 33, 20);
    u8g2.drawFrame(9, 37, 56, 11);
    u8g2.drawStr(22, 46, "START");
    //dibuja atras
    u8g2.drawFrame(102, 1, 23, 9);
    u8g2.setFont(u8g2_font_4x6_tr);
    u8g2.drawUTF8(104, 8, "Atrás");

    //si no se pulsa el joystick
    if (estado_joystick[0] == 1) {
      
      //a la izquierda
      if ((estado_joystick[1] == -1) && col>0 ) {
        col--;
        fila=0;
      }
      //a la derecha
      else if ((estado_joystick[1]) == 1 && col<1 ) {
        col++;
        //pasa a boton stop
        fila=1;
      }
      if(col==0){
        //hacia arriba
        if (estado_joystick[2] == 1 && fila > 0 &&  fila<=1){
        fila--;
        } 
        //hacia abajo
        else if (estado_joystick[2] == -1 && fila < 1 && fila>=0) {
          fila++;
        }
      }
      else if(col==1){
        //hacia arriba
        if (estado_joystick[2] == 1 && fila > 0){
        fila--;
        } 
        //hacia abajo
        else if (estado_joystick[2] == -1 && fila < 1 ) {
          fila++;
        }

      }
      if(col==0){
        if(fila==0){
        //selector en editar
          u8g2.drawBox(71, 24, 7, 7);
          
        } 
        else if(fila==1){
          //Cuando le das a start
          u8g2.drawBox(71, 39, 7, 7);
        }
        
        
      }
      else if(col==1){
        if(fila==0){
          //dibujar flecha atras
          u8g2.drawXBM(89, 3, 9, 7, image_Layer_5_bits);
        }
        else if(fila==1){
          //selector de stop
          u8g2.drawLine(89, 24, 89, 35);

          u8g2.drawLine(100, 42, 116, 42);

          u8g2.drawLine(100, 17, 116, 17);

          u8g2.drawLine(125, 25, 125, 36);
        
        }
      }
    }
    else if(estado_joystick[0] == 0){
      if(col<=0){
        if (fila <= 0){
          opcion = "Editar";
          seguir2=false;
        } 
        else if(fila>=1){
        //ejecuta a nivel de motores el entrenamiento con fila=1 FALTA
          iniciarEntrenamiento(copia_ent);
          
        }
        
      }
      else if (col>=1){
        if(fila<=0){
          seguir2=false;
          opcion="Atras"; // Si se pulsa el botón del joystick
        }
        else if(fila>=1){
        
          //parar motores
          stopMotor=true;
          seguir2=false;
          opcion="Atras";
        }
      }
    }


  //segun el boton actual muestra el selector en diferente boton
  //u8g2.drawStr(95, 59, "30 s");
  //u8g2.setFont(u8g2_font_4x6_tr);
  //u8g2.drawStr(41, 59, "Empieza en :");
  u8g2.sendBuffer();
  }
  }
  

  


// lista con los datos del entreamiento manual de izq a der : cant_golpes,vel_motor_sup,vel_motor_inf
int datos_ent_manual[3]={0};

//muestra velocidad motor y las bolas restantes

void mostrarEstadoEjercicio(int velocidad_actual, int repeticiones_totales, int contador_actual) {
  int pelotas_restantes = repeticiones_totales - contador_actual;
  if (pelotas_restantes < 0) pelotas_restantes = 0;

  u8g2.clearBuffer();

  // Marco
  u8g2.drawFrame(0, 0, 128, 64);

  // Texto
  u8g2.setFont(u8g2_font_6x12_tr);

  // Velocidad
  u8g2.drawStr(10, 20, "Velocidad motor:");
  u8g2.setCursor(110, 20);
  u8g2.print(velocidad_actual);
  u8g2.print("%");

  // Pelotas restantes
  u8g2.drawStr(10, 40, "Pelotas restantes:");
  u8g2.setCursor(110, 40);
  u8g2.print(pelotas_restantes);

  u8g2.sendBuffer();
}
//cambia un valor (numvalor) de una lista de datos entre unos valor max y min cada vez le suma o le resta (cambio)
void ejec_cambiar_valor_mod_manual(int (&lista_enteros)[],int numvalor,int cambio,int max,int min){

  //pasar cada valor de la lista a string
  String reps= String(lista_enteros[0]);
  String motor_sup=String(lista_enteros[1]);
  String motor_inf=String(lista_enteros[2]);
  //mientras no se pulse el joystic,se cmabia el valro subiendo y bajando
  update_joystick();
  while(estado_joystick[0] == 1){
    //mostrar menú entero
    reps= String(lista_enteros[0]);
    motor_sup=String(lista_enteros[1]);
    motor_inf=String(lista_enteros[2]);
    u8g2.clearBuffer();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    //Esqueleto
    u8g2.drawFrame(1, 20, 70, 11);
    u8g2.drawFrame(1, 33, 69, 11);
    u8g2.drawFrame(1, 8, 70, 11);
    u8g2.setFont(u8g2_font_4x6_tr);
    u8g2.drawStr(4, 16, "Cant Golpes");
    u8g2.drawStr(4, 28, "VelMotor Sup");
    u8g2.drawStr(4, 41, "VelMotor Inf");

    u8g2.drawStr(75, 28, motor_sup.c_str());
    u8g2.drawStr(76, 16, reps.c_str());
    u8g2.drawStr(75, 41, motor_inf.c_str());
    u8g2.drawFrame(69, 50, 46, 11);
    u8g2.drawFrame(7, 50, 46, 11);
    u8g2.drawStr(15, 58, "Start");
    u8g2.drawStr(79, 58, "Stop");
    //cuadrado del boton atras
    u8g2.drawFrame(102, 2, 23, 9);
    //str del boton atras
    u8g2.drawUTF8(104, 8, "Atrás");
    
    update_joystick();
    //subir
    if(estado_joystick[2] == 1){
      if(lista_enteros[numvalor]<=(max-cambio)){

        lista_enteros[numvalor]=lista_enteros[numvalor]+cambio;
      }

    }
    //bajar
    else if(estado_joystick[2] == -1){
      if(lista_enteros[numvalor]>=(min+cambio)){

        lista_enteros[numvalor]=lista_enteros[numvalor]-cambio;
      }

    }
  u8g2.sendBuffer();
  }
}

//campo es la variable de entrenamiento que se cambia ordenado de 0-4: reps,mot_sup,...angUD
void cambiar_valores_ejer_mod_entren(int (*ent)[5],int &ejer,int campo,int cambio,int max,int min){
  String reps= String(ent[ejer][0]);
  String motor_sup=String(ent[ejer][1]);
  String motor_inf=String(ent[ejer][2]);
  String motor_3= String(ent[ejer][3]);
  //String angRL= String(ent[ejer][3]);
  String angUD=String(ent[ejer][4]);
  String nejer="";
  //mientras no se pulse el joystic,se cmabia el valro subiendo y bajando
  update_joystick();
  while(estado_joystick[0] == 1){
    //mostrar menú entero
    reps= "Reps "+String(ent[ejer][0]);
    motor_sup="Motor_sup "+String(ent[ejer][1]);
    motor_inf="Motor_inf "+String(ent[ejer][2]);
    motor_3= "motor_3 "+String(ent[ejer][3]);
    //angRL= "AngRL "+String(ent[ejer][3]);
    angUD="AngUD "+String(ent[ejer][4]);
    nejer=String(ejer)+"/19";

    u8g2.clearBuffer();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_5x8_tr);
    //numero ejercicio actual
    u8g2.drawStr(95, 56, nejer.c_str());
    u8g2.setFont(u8g2_font_4x6_tr);
    //str del boton atras
    u8g2.drawUTF8(104, 8, "Atrás");
    u8g2.drawStr(6, 13, reps.c_str());
    u8g2.drawStr(6, 23, motor_sup.c_str());
    u8g2.drawStr(6, 32, motor_inf.c_str());
    u8g2.drawStr(6, 42, motor_3.c_str());
    u8g2.drawStr(6, 53, angUD.c_str());
    //boton save
    u8g2.setFont(u8g2_font_6x13_tr);
    u8g2.drawStr(95, 33, "Save");
    u8g2.drawFrame(92, 19, 30, 20);
    //cuadrado del boton atras
    u8g2.drawFrame(102, 2, 23, 9);
    
    
    update_joystick();
    //subir
    if(estado_joystick[2] == 1){
      if(ent[ejer][campo]<=(max-cambio)){

        ent[ejer][campo]=ent[ejer][campo]+cambio;
      }

    }
    //bajar
    else if(estado_joystick[2] == -1){
      if(ent[ejer][campo]>=(min+cambio)){

        ent[ejer][campo]=ent[ejer][campo]-cambio;
      }

    }
  u8g2.sendBuffer();
  }
}

void ejec_cambiar_ejer_mod_entren(int (*ent)[5],int &ejer,int cambio,int max,int min){

  String reps= String(ent[ejer][0]);
  String motor_sup=String(ent[ejer][1]);
  String motor_inf=String(ent[ejer][2]);
  String motor_3= String(ent[ejer][3]);
  //String angRL= String(ent[ejer][3]);
  String angUD=String(ent[ejer][4]);
  String nejer="";
  //mientras no se pulse el joystic,se cmabia el valro subiendo y bajando
  update_joystick();
  while(estado_joystick[0] == 1){
    //mostrar menú entero
    reps= "Reps "+String(ent[ejer][0]);
    motor_sup="Motor_sup "+String(ent[ejer][1]);
    motor_inf="Motor_inf "+String(ent[ejer][2]);
    motor_3= "motor_3 "+String(ent[ejer][3]);
    angUD="AngUD "+String(ent[ejer][4]);
    nejer=String(ejer)+"/19";

    u8g2.clearBuffer();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_5x8_tr);
    //numero ejercicio actual
    u8g2.drawStr(95, 56, nejer.c_str());
    u8g2.setFont(u8g2_font_4x6_tr);
    //str del boton atras
    u8g2.drawStr(104, 8, "Atras");
    u8g2.drawStr(6, 13, reps.c_str());
    u8g2.drawStr(6, 23, motor_sup.c_str());
    u8g2.drawStr(6, 32, motor_inf.c_str());
    u8g2.drawStr(6, 42, motor_3.c_str());
    u8g2.drawStr(6, 53, angUD.c_str());
    //boton save
    u8g2.setFont(u8g2_font_6x13_tr);
    u8g2.drawStr(95, 33, "Save");
    u8g2.drawFrame(92, 19, 30, 20);
    //cuadrado del boton atras
    u8g2.drawFrame(102, 2, 23, 9);
    
    
    update_joystick();
    //subir
    if(estado_joystick[2] == 1){
      if(ejer<=(max-cambio)){

        ejer=ejer+cambio;
      }

    }
    //bajar
    else if(estado_joystick[2] == -1){
      if(ejer>=(min+cambio)){

        ejer=ejer-cambio;
      }

    }
  u8g2.sendBuffer();
  }
}

//funcion que controla el movimiento de los motores que cambian los ángulos


void moveDegrees(int dirPin, int stepPin, 
                 float grados, 
                 volatile bool &detenerGiroHorario, 
                 volatile bool &detenerGiroAntihorario, 
                 float &grados_ini) 
{
  if (grados == 0) return;

  // Bloquear si el final de carrera correspondiente está activado
  if (grados > 0 && detenerGiroHorario) return;      
  if (grados < 0 && detenerGiroAntihorario) return;  

  // Configurar dirección
  digitalWrite(dirPin, grados > 0 ? HIGH : LOW);

  // Calcular pasos
  int steps = abs((int)(fabs(grados) * stepsPerRev / 360.0 + 0.5));

  for (unsigned long i = 0; i < steps; i++) {
    if ((grados > 0 && detenerGiroHorario) || (grados < 0 && detenerGiroAntihorario)) {
      break;  // detener inmediatamente si el botón se presiona
    }
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(pulseUs);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(pulseUs);
  }

  // Actualizar ángulo del motor correspondiente
  grados_ini += grados;
  if (grados_ini >= 360.0) grados_ini -= 360.0;
  if (grados_ini < 0.0) grados_ini += 360.0;
}

//funcion que se ejecuta al pulsar "Vertical" o Horizontal y que mueve el motor usando un determinado eje del joystick
void controlJoystickMotor(int dirPin, int stepPin, 
                          volatile bool &detenerGiroHorario, 
                          volatile bool &detenerGiroAntihorario, 
                          float &grados_ini,
                          int movimiento)   // <--- se pasa como parámetro,así se puede controlar con el movimiento/estado del joystic veritcal o horiz
{
  // Decidir movimiento
  if (movimiento == 1) {
    moveDegrees(dirPin, stepPin, 30.0, detenerGiroHorario, detenerGiroAntihorario, grados_ini);
  } 
  else if (movimiento == -1) {
    moveDegrees(dirPin, stepPin, -30.0, detenerGiroHorario, detenerGiroAntihorario, grados_ini);
  }
}
void ejecutar_cambio_angulos(String HoV){

  update_joystick();
  while(estado_joystick[0] == 1){
    float angV=grados_ini_V;
    float angH=grados_ini_H;
    // Convertir float a String con un decimal
    String angV_str = String(angV, 1);  
    String angH_str = String(angH, 1);
    //esqueleto interfaz
    u8g2.clearBuffer();
    u8g2.drawFrame(5, 11, 61, 14);
    u8g2.drawFrame(5, 28, 61, 13);
    u8g2.setFont(u8g2_font_5x8_tr);
    u8g2.drawStr(12, 21, "Vertical");
    u8g2.drawStr(11, 37, "Horizontal");
    u8g2.setFont(u8g2_font_6x13_tr);
    u8g2.drawStr(87, 39, angH_str.c_str());
    u8g2.drawStr(86, 23, angV_str.c_str());
    u8g2.drawFrame(5, 43, 61, 14);
    u8g2.setFont(u8g2_font_5x8_tr);
    u8g2.drawStr(15, 53, "Calibrar");
    u8g2.drawUTF8(104, 8, "Atrás");
    update_joystick();
    
    if(HoV=="V"){

      controlJoystickMotor(dirPinV, stepPinV, 
                       detenerGiroHorarioV, detenerGiroAntihorarioV, 
                       grados_ini_V,
                       estado_joystick[2]);   // << movimiento vertical
    }
    else if(HoV=="H"){

      controlJoystickMotor(dirPinH, stepPinH, 
                       detenerGiroHorarioH, detenerGiroAntihorarioH, 
                       grados_ini_H,
                       estado_joystick[2]);   // << movimiento horizontal
    }
    
  u8g2.sendBuffer();
  delay(100);
  }
  }
  
  


void menu_cambio_angulos(){

//el valor de cada campo se debe introducir antes o despues de esta funcion,solo dibuja el selector sin los valores.
  int fila=0;
  int col=0;
  float angV=grados_ini_V;
  float angH=grados_ini_H;
  bool seguir5=true;
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  //bucle de este menú
  while(seguir5){
    
    angV=grados_ini_V;
    angH=grados_ini_H;
    String angV_str = String(angV, 1);  
    String angH_str = String(angH, 1);
    
    //esqueleto interfaz
    u8g2.clearBuffer();
    u8g2.drawFrame(5, 11, 61, 14);
    u8g2.drawFrame(5, 28, 61, 13);
    u8g2.setFont(u8g2_font_5x8_tr);
    u8g2.drawStr(12, 21, "Vertical");
    u8g2.drawStr(11, 37, "Horizontal");
    u8g2.setFont(u8g2_font_6x13_tr);
    u8g2.drawStr(87, 39, angH_str.c_str());
    u8g2.drawStr(86, 23, angV_str.c_str());
    u8g2.drawFrame(5, 43, 61, 14);
    u8g2.setFont(u8g2_font_5x8_tr);
    u8g2.drawStr(15, 53, "Calibrar");
    u8g2.drawUTF8(104, 8, "Atrás");
    update_joystick();
    //si no se pulsa el joystick
    if (estado_joystick[0] == 1) {
      
      //a la izquierda
      if ((estado_joystick[1] == -1) && col>0 && col>=1) {
        col--;
        fila=0;
      }
      //a la derecha
      else if ((estado_joystick[1]) == 1 && col<1 && col>=0) {
        col++;
        //pasa a boton stop
        fila=1;
      }
      if(col==0){
        //hacia arriba
        if (estado_joystick[2] == 1 && fila > 0 &&  fila<=2){
        fila--;
        } 
        //hacia abajo
        else if (estado_joystick[2] == -1 && fila < 2 && fila>=0) {
          fila++;
        }
      }
      else if(col==1){
        fila=0;

      }
      
      //segun el boton actual muestra el selector en diferente boton
      if(col==0){
        if(fila==0){
          
           u8g2.drawBox(72, 15, 6, 6);
        } 
        else if(fila==1){
          u8g2.drawBox(72, 31, 6, 6);
          
        }
        else if(fila==2){
         u8g2.drawBox(72, 47, 6, 6);
        }
      
      }
      else if(col==1){
        if(fila==0){
          //dibujar flecha atras
        u8g2.drawXBM(89, 3, 9, 7, image_Layer_5_bits);
        }
        
      }


    }
    //si se pulsan los botones
    else if(estado_joystick[0] == 0){
      if(col==0){

        //motor Vertical
        if (fila == 0){
          ejecutar_cambio_angulos("V");
          
        }
        //motor Horizontal
        else if (fila == 1){
          ejecutar_cambio_angulos("H");
        
        }

        else if (fila == 2){
          grados_ini_V=0.0;
          grados_ini_H=0.0;

        }
       
      
      }
      else if (col==1){

        //pulsa atras
        if(fila==0){
          seguir5=false;
          estado_menu = "Inicio";
        } 
      }
  
  
    }
  u8g2.sendBuffer();
}




}

void ejecutar_modo_manual(){
  //el valor de cada campo se debe introducir antes o despues de esta funcion,solo dibuja el selector sin los valores.
  int fila=0;
  int col=0;
  bool seguir2=true;
  bool stop=false;
  int pelotas_actuales = 0;
  String reps= String(datos_ent_manual[0]);
  String motor_sup=String(datos_ent_manual[1]);
  String motor_inf=String(datos_ent_manual[2]);
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  //copio el array del entrenamiento para no tener que consultarlo dentro de la tarea directamete
  int (*copia_ent)[5] = new int[20][5];
  memcpy(copia_ent, datos_ent_manual, sizeof(int) * 20 * 5);
  while(seguir2){
    reps= String(datos_ent_manual[0]);
    motor_sup=String(datos_ent_manual[1]);
    motor_inf=String(datos_ent_manual[2]);
    portENTER_CRITICAL(&mux);
    pelotas_actuales = contador_pelotas;
    portEXIT_CRITICAL(&mux);
    u8g2.clearBuffer();
    //Esqueleto
    u8g2.drawFrame(1, 20, 70, 11);
    u8g2.drawFrame(1, 33, 69, 11);
    u8g2.drawFrame(1, 8, 70, 11);
    u8g2.setFont(u8g2_font_4x6_tr);
    u8g2.drawStr(4, 16, "Cant Golpes");
    u8g2.drawStr(4, 28, "VelMotor Sup");
    u8g2.drawStr(4, 41, "VelMotor Inf");

    u8g2.drawStr(75, 28, motor_sup.c_str());
    u8g2.drawStr(76, 16, reps.c_str());
    u8g2.drawStr(75, 41, motor_inf.c_str());
    u8g2.drawFrame(69, 50, 46, 11);
    u8g2.drawFrame(7, 50, 46, 11);
    u8g2.drawStr(15, 58, "Start");
    u8g2.drawStr(79, 58, "Stop");
    //cuadrado del boton atras
    u8g2.drawFrame(102, 2, 23, 9);
    //str del boton atras
    u8g2.drawUTF8(104, 8, "Atrás");
    //pelotas restantes

    String pelotasStr = String(pelotas_actuales);
    u8g2.setFont(u8g2_font_4x6_tr);
    u8g2.drawStr(4, 70, "Bolas:");
    u8g2.drawStr(40, 70, pelotasStr.c_str());

    update_joystick();
    //si no se pulsa el joystick
    if (estado_joystick[0] == 1) {
      
      //a la izquierda
      if ((estado_joystick[1] == -1) && col>0 && col>=1) {
        col--;
        fila=0;
      }
      //a la derecha
      else if ((estado_joystick[1]) == 1 && col<1 && col>=0) {
        col++;
        //pasa a boton stop
        fila=1;
      }
      if(col==0){
        //hacia arriba
        if (estado_joystick[2] == 1 && fila > 0 &&  fila<=3){
        fila--;
        } 
        //hacia abajo
        else if (estado_joystick[2] == -1 && fila < 3 && fila>=0) {
          fila++;
        }
      }
      else if(col==1){
        //hacia arriba
        if (estado_joystick[2] == 1 && fila > 0 && fila<=1){
        fila--;
        } 
        //hacia abajo
        else if (estado_joystick[2] == -1 && fila < 1 && fila>=0) {
          fila++;
        }

      }
      
      //segun el boton actual muestra el selector en diferente boton
      if(col==0){
        if(fila==0){
          //cuadrado valor cantgolpes
          u8g2.drawFrame(73, 8, 15, 11);
          //cuadrado cantgolpes
          u8g2.drawBox(91, 11, 5, 5);
        } 
        else if(fila==1){
          //cuadrado vel motor sup
          //rectangulo valor VelmotorSup
          u8g2.drawFrame(73, 20, 15, 11);
          u8g2.drawBox(91, 23, 5, 5);
        }
        else if(fila==2){
          //cuadrado vel modor inf 
          u8g2.drawBox(91, 35, 5, 5);
          //cuadrado val velmotinf
          u8g2.drawFrame(73, 33, 15, 11);
        }
        else if(fila==3){
          //cuadrado start
          u8g2.drawBox(43, 53, 5, 5);
        }
      }
      else if(col==1){
        if(fila==0){
          //dibujar flecha atras
        u8g2.drawXBM(89, 3, 9, 7, image_Layer_5_bits);
        }
        else if(fila==1){
          //cuadrado boton stop
          u8g2.drawBox(105, 53, 5, 5);
        
        }
      }


    }
    else if(estado_joystick[0] == 0){
      if(col==0){
        if (fila == 0){
          //resps maximas=20
          ejec_cambiar_valor_mod_manual(datos_ent_manual,0,1,20,0);
          

        }
        else if (fila == 1){

          ejec_cambiar_valor_mod_manual(datos_ent_manual,1,1,100,0);
        
        }

        else if (fila == 2){

          ejec_cambiar_valor_mod_manual(datos_ent_manual,2,1,100,0);
        
        }
        //Se pulsa Start
        else if (fila == 3){

          //se invoca la tarea en el otro nucleo
          iniciarEjercicio(float(datos_ent_manual[1]),float(datos_ent_manual[2]),datos_ent_manual[0]);

  
        
        }
      
      }
      else if (col==1){

        //pulsa atras
        if(fila==0){
          seguir2=false;
          estado_menu = "Inicio";
        }
        // Si se pulsa stop
        else if(fila==1){
        
          //parar los motores (velsup=inf=0)
        
        stopMotor=true;
        
        }
        
    }
  
  
  }
  u8g2.sendBuffer();
  
}
}

//recibe las coordenadas de la posicionXY real desde sit ref centrado en fondo
// pista centro y mirando acia la red con el eje y y hacia la der con ejex
void ejecutar_localizacion(){
  fila=0;
  col=0;
  float posicionXY[2]={0.0,0.0};
  if(pos_robot_real!=NULL){
    posicionXY[0] = pos_robot_real[0];
    posicionXY[1] = pos_robot_real[1];
  }
  
  int posx=31+(int)(posicionXY[0]*4.81)*(-1);
  int posy=69-(int)(posicionXY[1]*4.81)*(1);
  int posdibujox=posx-3;
  int posdibujoy=posy-3;
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  u8g2.clearBuffer();
  bool seleccionar=false;
  bool seguir=true;
  //LOGICA DE INTERACCION
  while(seguir){
    
    //actualiza las mediciones del joystick
    update_joystick();
    //miro estado vertical(arriba,abajo o centro) y actuo muestro el selector y hago acciones
    //si no se está pulsando=0  y esté en modo seleccionar (para no hacer esto al eatar pulsando para dejar porobot fija) mostrar solo el selector
    
    //si esta en modo seleccion botones
    while(seleccionar){
      u8g2.clearBuffer();
      u8g2.drawFrame(102, 2, 23, 9);
      u8g2.setFont(u8g2_font_4x6_tr);
      u8g2.drawUTF8(104, 8, "Atrás");
      u8g2.drawFrame(5, 11, 65, 43);
      u8g2.drawLine(5, 31, 46, 31);
      u8g2.drawLine(24, 1, 24, 62);
      u8g2.drawLine(23, 1, 23, 62);
      u8g2.drawLine(46, 11, 46, 52);
      u8g2.drawLine(69, 53, 69, 57);
      u8g2.drawLine(68, 57, 5, 57);
      u8g2.drawLine(5, 5, 69, 5);
      u8g2.drawLine(5, 5, 5, 11);
      u8g2.drawLine(69, 10, 69, 6);
      u8g2.drawLine(5, 57, 5, 53);
      u8g2.drawStr(97, 33, "Guardar");
      u8g2.drawFrame(94, 26, 31, 10);
      update_joystick();
      //Verifico pulsacion del joystick
      if(estado_joystick[0]==0){
        //apretar en posicionado
        if(col==0){

          seleccionar=false;
        
        }
        //apretar en atras
        else if(col==1 && fila==0){
          seleccionar=false;
          seguir=false;
          estado_menu="Inicio";



        }
        //apretar en guardar
        else if(col==1 && fila==1){
        //info estado
        memoria.begin("lanzador", false);
        //nombre de guardado,referencia a la variable que se quiere guardar,tamaño en bytes
        memoria.putBytes("posxrobot", &posicionXY[0], sizeof(posicionXY[0]));
        memoria.putBytes("posyrobot", &posicionXY[1], sizeof(posicionXY[1]));
        memoria.end();
        u8g2.drawStr(85, 57, "Guardada!!");
        pos_robot_real[0]=posicionXY[0];
        pos_robot_real[1]=posicionXY[1];
        }
      }
      //hacia arriba
      else if( estado_joystick[2]==1){
        //si no el elemento de más arriba
        if(col==1 && fila==1){

          fila=0;
          
        } 
      }
      //hacia abajo
      else if( estado_joystick[2]==-1){
        //si no el elemento de más arriba
        if(col==1 && fila==0){

         fila=1;
        
          
        } 
      }
      else if( estado_joystick[1]==-1){
        //hacia izquierda si es la columna 1
        if(col==1){
         //en posicionado
          col=0;
         //si estamos en columna 0,la fila es siempre 0 ya que solo hay 1 elelemto(posicionado)
          fila=0;
          
        } 
      }
      else if( estado_joystick[1]==1){
        //hacia derecha si es la columna 0
        if(col==0){

         //se pone el selector en "guardar"
         col=1;
         fila=1;
         
        } 
      }
      if(col==1 && fila==1){
        //flecha guarddar
         u8g2.drawXBM(80, 28, 9, 7, image_Layer_5_bits);

      }
      else if(col==1 && fila==0){
        //flecha atras
          u8g2.drawXBM(89, 3, 9, 7, image_Layer_5_bits);
      }
      else if(col==0){
      //enmarcado de campo
          u8g2.drawLine(1, 26, 1, 32);

          u8g2.drawLine(34, 2, 40, 2);

          u8g2.drawLine(34, 60, 40, 60);

          u8g2.drawLine(75, 28, 75, 34);


      }
      

      
      //FALTA CONSULTAR ESTADO DE FILA,COL Y SI SE PULSA DEPENDE QUÉ ZONA HAGA ALGO,ademas,falta probar el posicionado y ver qué falla
    u8g2.sendBuffer();
    }
    
    //miramos estado joystick cuando esta moviendo la posición del robot

    while(!seleccionar){
      u8g2.clearBuffer();
      u8g2.drawFrame(102, 2, 23, 9);
      u8g2.setFont(u8g2_font_4x6_tr);
      u8g2.drawUTF8(104, 8, "Atrás");
      u8g2.drawFrame(5, 11, 65, 43);
      u8g2.drawLine(5, 31, 46, 31);
      u8g2.drawLine(24, 1, 24, 62);
      u8g2.drawLine(23, 1, 23, 62);
      u8g2.drawLine(46, 11, 46, 52);
      u8g2.drawLine(69, 53, 69, 57);
      u8g2.drawLine(68, 57, 5, 57);
      u8g2.drawLine(5, 5, 69, 5);
      u8g2.drawLine(5, 5, 5, 11);
      u8g2.drawLine(69, 10, 69, 6);
      u8g2.drawLine(5, 57, 5, 53);
      u8g2.drawStr(97, 33, "Guardar");
      u8g2.drawFrame(94, 26, 31, 10);
      update_joystick();
      if(estado_joystick[0]==0){
        
        seleccionar=true;
      }
      else{
      //si se mueve hacia arriba se mueve en + 0.24 cm en x
        if(estado_joystick[2]==1){

          posicionXY[0]=posicionXY[0]+0.24;

        
      }
        else if(estado_joystick[2]==-1){

        posicionXY[0]=posicionXY[0]-0.24;
        
      }
      //movimientos en horizontal se mueve en eje y(hacia la red)
        if(estado_joystick[1]==-1){

          posicionXY[1]=posicionXY[1]+0.24;
        
        }
        else if(estado_joystick[1]==1){

          posicionXY[1]=posicionXY[1]-0.24;
        
      }
      //dibujar localizacion
      //empezar localizacion
      //en la realidad

      //en el 0,0 de nuestro sistema de coordenadas centrado en 
      //el centro del fondo de pista cn ejey hacia red y eje x hacia la derecha
      //en el centro : u8g2.drawXBM(66, 29, 7, 7, image_Layer_8_bits);
      //pos y son los pixeles de las columnas en el display
      //posx de las filas
      //en la pantalla mueve de 5 en 5 pixeles=20,75 cm
    
      //pasar de posiciones en x e y en la realidad a celdas de 
      //la pantalla con el sitema de coordenadas en el mismo sitio pero en dibujo(en el campo de tenis)
      posx=31+(int)(posicionXY[0]*4.81)*(-1);
      posy=69-(int)(posicionXY[1]*4.81)*(1);
    
      if((posy<=69 && posy>=26)&& (posx>=5 && posx<=58)){
        //¿cuantos pixeles son 1 metro?
        //X que muestra la posicion del lanzador
        //centrar el dibujo de la X para ello tenemos que dibujar desde un exteremo no desde el centro
        posdibujox=posx-3;
        posdibujoy=posy-3;
      
      }
      
    }
    u8g2.drawXBM(posdibujoy, posdibujox, 7, 7, image_Layer_8_bits);
    u8g2.sendBuffer();
    
  }

 }
       
}





// Función para detectar oscuridad usando millis()
bool detectarOscuridad() {
    unsigned long tiempoActual = millis();
    
    // Solo lee el sensor si ha pasado el intervalo
    if (tiempoActual - ultimoTiempoLectura >= INTERVALO_LECTURA) {
        ultimoTiempoLectura = tiempoActual;  // Actualizar tiempo de última lectura
        
        int luz = analogRead(LDR_PIN);  // Leer sensor
        //Serial.print("Lectura luz: ");
        //Serial.println(luz);

        return (luz < UMBRAL_OSCURIDAD); // Retorna true si está oscuro
    }

    return false;  // Si no ha pasado el intervalo, mantiene el último estado
}


//HAY QUE CREAR LA TAREA DE MOVER LOS MOTORES DE FORMA PARALELA EN OTRO
// NUCLEO para que las constantes comprovaciones en las funciones que se ejecutan dentro del loop no afecten a la velocidad del pwm.

// === ESTRUCTURA DE PARÁMETROS PARA LA TAREA ===
struct MotorParams {
  float velocidad1;
  float velocidad2;
  int repeticiones;
};

// === VARIABLES GLOBALES ===
bool estado_ant = false;
bool started_exercice = false;
TaskHandle_t motorTaskHandle = NULL;


//funcion que creea e envoca la tarea de activar motores hasta cierto numero de pelotas en el otro nucleo
void iniciarEjercicio(float velocidad1,float velocidad2, int repeticiones) {
  // Evitar crear otra tarea si ya hay una corriendo
  if (motorTaskHandle != NULL) {
    return;
  }
  stopMotor=false;

  // Asignar parámetros dinámicos
  MotorParams* params = new MotorParams;
  params->velocidad1 = velocidad1;
  params->velocidad2 = velocidad2;
  params->repeticiones = repeticiones;

  // Crear tarea en el núcleo 1
  xTaskCreatePinnedToCore(
    tarea_pelotas_y_motor,     // Función de la tarea
    "PelotasYMotor",           // Nombre
    4096,                      // Stack size
    (void*)params,             // Parámetros
    1,                         // Prioridad
    &motorTaskHandle,          // Handle (global)
    1                          // Núcleo 1
  );

  
}


//no usa un bucle,hay que controlar la fucnion desde bfuera con un bucle
void contar_pelotas(volatile int &conta,bool &estado_ant){
  //si no detecta luz 
  if(detectarOscuridad()){
    //si nel estado anterior era false es decir habia luz y ahora no(true)
    if(estado_ant==false){

      portENTER_CRITICAL(&mux);
      conta=conta+1;
      portEXIT_CRITICAL(&mux);

    }
    estado_ant=true;
    
  }
  else{
    estado_ant=false;
  }
  //si se ha contado una pelota entonces ponemos como estado 
  //ant=false ya que la pelota ya ha caido y esta el photoreceptor recibiendo toda la luz
}

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
  else if (pin == pin_motor2) vel_motor2 = porcentaje;
  else if (pin == pin_motor3) vel_motor3 = porcentaje;
}


// Rampa S-curve para 2 motores sincronizados + 1 motor independiente
void rampaMotores(float velIni1, float velFin1,
                  float velIni2, float velFin2,
                  float velIni3, float velFin3) {

  // ======= Motores 1 y 2 (sincronizados) =======
  float delta1 = fabs(velFin1 - velIni1);
  float delta2 = fabs(velFin2 - velIni2);
  float deltaMax = max(delta1, delta2);

  // ======= Motor 3 (independiente) =======
  float delta3 = fabs(velFin3 - velIni3);

  // Si no hay variación significativa
  if (deltaMax < 0.1f && delta3 < 0.1f) {
    setPWM(pin_motor1, velFin1, PWM_MIN_MOTOR1);
    setPWM(pin_motor2, velFin2, PWM_MIN_MOTOR2);
    setPWM(pin_motor3, velFin3, PWM_MIN_MOTOR3);
    return;
  }

  // ======= Tiempo y pasos para motores 1 y 2 =======
  int t_rampa12 = (int)(T_RAMPA_MAX * (deltaMax / 90.0f));
  if (t_rampa12 < 50) t_rampa12 = 50;

  int pasos12 = max(20, (int)(deltaMax * 2.5f));
  float dt12 = (float)t_rampa12 / pasos12;

  // ======= Tiempo y pasos para motor 3 =======
  int t_rampa3 = (int)(T_RAMPA_MAX * (delta3 / 90.0f));
  if (t_rampa3 < 50) t_rampa3 = 50;

  int pasos3 = max(20, (int)(delta3 * 2.5f));
  float dt3 = (float)t_rampa3 / pasos3;

  // ======= Usar la rampa más larga como referencia =======
  int pasosMax = max(pasos12, pasos3);
  float dt = min(dt12, dt3);  // paso temporal más fino

  for (int i = 0; i <= pasosMax; i++) {
    // Motores 1 y 2 (sincronizados)
    float s12 = 1.0f;
    if (i <= pasos12)
      s12 = 0.5f * (1.0f - cosf(PI * ((float)i / pasos12)));

    float vel1 = velIni1 + (velFin1 - velIni1) * s12;
    float vel2 = velIni2 + (velFin2 - velIni2) * s12;

    // Motor 3 (independiente)
    float s3 = 1.0f;
    if (i <= pasos3)
      s3 = 0.5f * (1.0f - cosf(PI * ((float)i / pasos3)));

    float vel3 = velIni3 + (velFin3 - velIni3) * s3;

    // Aplicar PWM
    setPWM(pin_motor1, vel1, PWM_MIN_MOTOR1);
    setPWM(pin_motor2, vel2, PWM_MIN_MOTOR2);
    setPWM(pin_motor3, vel3, PWM_MIN_MOTOR3);

    delay((int)dt);
  }
}




// Función que maneja la detección de pelotas y el control del motor en (el NÚCLEO 1)
// === TAREA EN NÚCLEO 1 ===
void tarea_pelotas_y_motor(void *pvParameters) {
  MotorParams *params = (MotorParams *)pvParameters;

  portENTER_CRITICAL(&mux);
  contador_pelotas = 0;
  portEXIT_CRITICAL(&mux);
  
  rampaMotores(vel_motor1, params->velocidad1, vel_motor2, params->velocidad2,vel_motor3, 60.0);
  while (contador_pelotas < params->repeticiones) {
    contar_pelotas(contador_pelotas, estado_ant);
    delay(200);  // Delay pequeño para estabilidad
    if (stopMotor) {

      break;  // Salir del bucle
    }
  }
  //parar los motores
  rampaMotores(vel_motor1, 0.0f, vel_motor2, 0.0f,vel_motor3, 0.0f);
  repeticiones_completadas = true;
  started_exercice = false;
  //delete params;
  motorTaskHandle = NULL;
  vTaskDelete(NULL);
 
  // Cuando se completa el ejercicio

  
  
}

//Tarea de entrenamiento completo
TaskHandle_t entrenamientoHandle = NULL;  // Handle global opcional

void tarea_entrenamiento_directo(void *pvParameters) {
  int (*ent_cop)[5] = (int (*)[5])pvParameters;
  portENTER_CRITICAL(&mux);
  stopMotor = false;
  portEXIT_CRITICAL(&mux);
  for (int i = 0; i < 20; i++) {
    int rep = ent_cop[i][0];
    //superior
    float vel1 = float(ent_cop[i][1]);
    //inferior
    float vel2 = float(ent_cop[i][2]);
    float vel3 = float(ent_cop[i][3]);


    if (rep <= 0) break;  // Fin de la secuencia

    portENTER_CRITICAL(&mux);
    contador_pelotas = 0;
    portEXIT_CRITICAL(&mux);
    rampaMotores(vel_motor1, vel1, vel_motor2, vel2,vel_motor3, vel3);

    while (contador_pelotas < rep) {
      contar_pelotas(contador_pelotas, estado_ant);
      delay(200);  // Anti rebote / lectura estable

      if (stopMotor) {
        rampaMotores(vel_motor1, 0.0f, vel_motor2, 0.0f,vel_motor3, 0.0f);
        //
        //delete[] ent_cop;
        entrenamientoHandle = NULL;
        vTaskDelete(NULL);  // Finalizar tarea
        return;
      }
    }

    rampaMotores(vel_motor1, 0.0f, vel_motor2, 0.0f,vel_motor3, 0.0f);
  }


  
  //delete[] ent_cop;
  entrenamientoHandle = NULL;
  vTaskDelete(NULL);
}


void iniciarEntrenamiento(int ent[20][5]) {
  // No lanzar si ya está en ejecución (opcional)
  if (entrenamientoHandle != NULL) return;
  xTaskCreatePinnedToCore(
    tarea_entrenamiento_directo,     // La tarea que recorre el array
    "EntrenamientoDirecto",          // Nombre de la tarea
    4096,                            // Tamaño de la pila
    (void*)ent,                      // Puntero al array
    1,                               // Prioridad
    &entrenamientoHandle,            // Handle para poder controlarla
    1                                // Núcleo 1 (secundario)
  );
}






void setup(void) {

 
  //Recuperar valor guardado de la pos robot
  //solo lectura 
  bool stopMotor=false;
  memoria.begin("lanzador", true);  // Modo solo lectura
  float posx = 0.0;
  memoria.getBytes("posxrobot", &posx, sizeof(posx));
  pos_robot_real[0]=posx;
  float posy = 0.0;
  memoria.getBytes("posyrobot", &posy, sizeof(posy));
  pos_robot_real[1]=posy;
  memoria.end();
  // Configuración de pines
  //DEL JOYSTICK
  pinMode(SW_PIN, INPUT_PULLUP);  // Botón con resistencia interna pull-up
  pinMode(VRX_PIN, INPUT);
  pinMode(VRY_PIN, INPUT);
  //DE LOS FINALES DE CARRERA
  pinMode(BOTON_VA, INPUT_PULLUP);
  pinMode(BOTON_VB, INPUT_PULLUP);
  pinMode(BOTON_HA, INPUT_PULLUP);
  pinMode(BOTON_HB, INPUT_PULLUP);
  //DE LOS MOTORES NEMA17
  pinMode(dirPinV, OUTPUT);
  pinMode(stepPinV, OUTPUT);
  pinMode(dirPinH, OUTPUT);
  pinMode(stepPinH, OUTPUT);
  // Configuración de interrupciones (una sola por pin, con CHANGE)
  attachInterrupt(digitalPinToInterrupt(BOTON_VA), interrupcionBotonAV, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BOTON_VB), interrupcionBotonBV, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BOTON_HA), interrupcionBotonAH, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BOTON_HB), interrupcionBotonBH, CHANGE);

  

  // Configuración PWM de los RS775
  ledcAttach(pin_motor1, frec, resolucion);
  ledcAttach(pin_motor2, frec, resolucion);

  setPWM(pin_motor1, 0, PWM_MIN_MOTOR1);
  setPWM(pin_motor2, 0, PWM_MIN_MOTOR2);

  // Cambiar los pines I²C si no usas los predeterminados
  Wire.begin(21, 22); // SDA = 21, SCL = 22
  u8g2.begin();       // Inicializamos la pantalla
  // Recuperar las matrices desde la memoria flash
  recuperarLista(ent1, "entrenamiento1");
  recuperarLista(ent2, "entrenamiento2");
  recuperarLista(ent3, "entrenamiento3");

  //no hay pelota inicialmente,recibe toda la luz del led
  bool estado_ant=false;
  int cont_pelotas=0;

}


void loop(void) {
  //la primera vez mostrar animacion
  if(contanim==0){

    mostrar_anim_inicio();

  }
  contanim=1;

  if(estado_menu=="Inicio") {

    //ejecutar menu de inicio
    ejecutar_menuprincipal();
  }
  else if (estado_menu=="Localizacion"){

    ejecutar_localizacion();
    
  }
  else if (estado_menu=="Entrenam"){
    ejecutar_menu_entrenamientos();
    
  }
  
  else if (estado_menu=="Modo_Manual"){
    
    ejecutar_modo_manual();
  }
  else if (estado_menu=="Angulos"){
    
    menu_cambio_angulos();
  }
  else if(estado_menu.substring(0, 3)=="ENT"){
  //convierto a int el char 3
    int valor = estado_menu[3] - '0';
    ejecutar_entrenamiento(valor);
    
  }
  
  
  
}

