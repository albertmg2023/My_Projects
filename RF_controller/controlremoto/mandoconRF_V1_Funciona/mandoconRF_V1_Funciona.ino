#include <U8g2lib.h>
#include <Wire.h>
#include <HardwareSerial.h>

static const unsigned char image_arrow_left_bits[] = {0x04,0x02,0x7f,0x02,0x04};
static const unsigned char image_download_bits[] = {0x10,0x20,0x7f,0x20,0x10};

// Pantalla OLED SSD1306 128x32
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

// HC-12 UART2, RX=19, TX=18
HardwareSerial hc12(2);

// Pines de botones
const int BTN_CHANGE    = 32;  // Botón de cambio de menú (no usado ahora)
const int BTN_START     = 33;  // Botón principal de selección/confirmación
const int BTN_ABAJO     = 14;
const int BTN_IZQUIERDA = 12;
const int BTN_ARRIBA    = 25;
const int BTN_DERECHA   = 4;

// Variables de submenús
int angVertical = 0;
int angHorizontal = 0;
int efectoLiftado = 0;
int efectoCortado = 0;
int velSup = 0;
int velInf = 0;
int tiempoBol = 2;

// --------------------------------------------------
// Setup
// --------------------------------------------------
void setup() {
  display.begin();
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB08_tr);
  display.drawStr(0, 15, "Sistema iniciado");
  display.sendBuffer();
  delay(2000);

  pinMode(BTN_CHANGE, INPUT_PULLUP);
  pinMode(BTN_START, INPUT_PULLUP);
  pinMode(BTN_ABAJO, INPUT_PULLUP);
  pinMode(BTN_IZQUIERDA, INPUT_PULLUP);
  pinMode(BTN_ARRIBA, INPUT_PULLUP);
  pinMode(BTN_DERECHA, INPUT_PULLUP);

  hc12.begin(9600, SERIAL_8N1, 18, 19); // RX=19, TX=18
}

// --------------------------------------------------
// Loop principal
// --------------------------------------------------
void loop() {
  showMenuPrincipal();
}

// --------------------------------------------------
// Menú principal
// --------------------------------------------------
void showMenuPrincipal() {
  const char* menuItems[] = {"ANGULOS", "EFECTO", "VELOCIDAD", "INTERVALO"};
  int selectedMenu = 0;

  while(true){
    display.clearBuffer();
    display.setFont(u8g2_font_6x12_tr);
    display.setBitmapMode(1);
    display.setCursor(45, 15);
    display.drawXBM(33, 24, 7, 5, image_arrow_left_bits);
    display.drawXBM(90, 25, 7, 5, image_download_bits);
    display.print(menuItems[selectedMenu]);
    display.sendBuffer();

    // Navegación
    if(digitalRead(BTN_DERECHA)==LOW){ selectedMenu=(selectedMenu+1)%4; delay(200);}
    if(digitalRead(BTN_IZQUIERDA)==LOW){ selectedMenu=(selectedMenu-1+4)%4; delay(200);}
    if(digitalRead(BTN_START)==LOW){
      switch(selectedMenu){
        case 0: showSubmenuAngulos(); break;
        case 1: showSubmenuEfecto(); break;
        case 2: showSubmenuVelocidad(); break;
        case 3: showSubmenuIntervalo(); break;
      }
      delay(300);
    }
  }
}

// --------------------------------------------------
// Submenú ANGULOS
// --------------------------------------------------
void showSubmenuAngulos(){
  int selRow=0, selCol=0; bool editing=false; 
  int numRows=2, totalRows=2;

  while(true){
    display.clearBuffer();
    display.setFont(u8g2_font_6x12_tr);
    display.setBitmapMode(1);

    // Dibujar elementos
    display.setCursor(5,12); display.print("V: "); display.print(angVertical);
    display.setCursor(70,12); display.print("ATRAS");
    display.setCursor(5,28); display.print("H: "); display.print(angHorizontal);
    display.setCursor(70,28); display.print("ENVIAR");

    // Resaltado del selector
    if(selRow==0 && selCol==0) drawSelector(5,12);
    if(selRow==0 && selCol==1) drawSelector(70,12);
    if(selRow==1 && selCol==0) drawSelector(5,28);
    if(selRow==1 && selCol==1) drawSelector(70,28);

    display.sendBuffer();

    // Navegación
    if(!editing){
      if(digitalRead(BTN_ARRIBA)==LOW){ selRow=(selRow-1+totalRows)%totalRows; delay(150);}
      if(digitalRead(BTN_ABAJO)==LOW){ selRow=(selRow+1)%totalRows; delay(150);}
      if(digitalRead(BTN_IZQUIERDA)==LOW){ selCol=(selCol-1+2)%2; delay(150);}
      if(digitalRead(BTN_DERECHA)==LOW){ selCol=(selCol+1)%2; delay(150);}
    } else {
      if(selCol==0){
        if(selRow==0){ if(digitalRead(BTN_ARRIBA)==LOW) angVertical=min(angVertical+1,60);
                        if(digitalRead(BTN_ABAJO)==LOW) angVertical=max(angVertical-1,0);}
        if(selRow==1){ if(digitalRead(BTN_ARRIBA)==LOW) angHorizontal=min(angHorizontal+1,20);
                        if(digitalRead(BTN_ABAJO)==LOW) angHorizontal=max(angHorizontal-1,0);}
        delay(100);
      }
    }

    // Confirmar con BTN_START
    if(digitalRead(BTN_START)==LOW){
      if(selCol==0) editing=!editing;
      else if(selCol==1 && selRow==0) return; // ATRÁS
      else if(selCol==1 && selRow==1) sendAngulos(); // ENVIAR
      delay(200);
    }
  }
}

// --------------------------------------------------
// Submenú EFECTO
// --------------------------------------------------
void showSubmenuEfecto(){
  int selRow=0, selCol=0; bool editing=false; 
  int numRows=2, totalRows=2;

  while(true){
    display.clearBuffer();
    display.setFont(u8g2_font_6x12_tr);
    display.setBitmapMode(1);

    display.setCursor(5,12); display.print("Lift: "); display.print(efectoLiftado);
    display.setCursor(70,12); display.print("ATRAS");
    display.setCursor(5,28); display.print("Cort: "); display.print(efectoCortado);
    display.setCursor(70,28); display.print("ENVIAR");

    if(selRow==0 && selCol==0) drawSelector(5,12);
    if(selRow==0 && selCol==1) drawSelector(70,12);
    if(selRow==1 && selCol==0) drawSelector(5,28);
    if(selRow==1 && selCol==1) drawSelector(70,28);

    display.sendBuffer();

    if(!editing){
      if(digitalRead(BTN_ARRIBA)==LOW){ selRow=(selRow-1+totalRows)%totalRows; delay(150);}
      if(digitalRead(BTN_ABAJO)==LOW){ selRow=(selRow+1)%totalRows; delay(150);}
      if(digitalRead(BTN_IZQUIERDA)==LOW){ selCol=(selCol-1+2)%2; delay(150);}
      if(digitalRead(BTN_DERECHA)==LOW){ selCol=(selCol+1)%2; delay(150);}
    } else {
      if(selCol==0){
        if(selRow==0){ if(digitalRead(BTN_ARRIBA)==LOW){efectoLiftado=min(efectoLiftado+1,100); efectoCortado=0;}
                        if(digitalRead(BTN_ABAJO)==LOW){efectoLiftado=max(efectoLiftado-1,0); efectoCortado=0;}}
        if(selRow==1){ if(digitalRead(BTN_ARRIBA)==LOW){efectoCortado=min(efectoCortado+1,100); efectoLiftado=0;}
                        if(digitalRead(BTN_ABAJO)==LOW){efectoCortado=max(efectoCortado-1,0); efectoLiftado=0;}}
        delay(100);
      }
    }

    if(digitalRead(BTN_START)==LOW){
      if(selCol==0) editing=!editing;
      else if(selCol==1 && selRow==0) return;
      else if(selCol==1 && selRow==1) sendEfecto();
      delay(200);
    }
  }
}

// --------------------------------------------------
// Submenú VELOCIDAD
// --------------------------------------------------
void showSubmenuVelocidad(){
  int selRow=0, selCol=0; bool editing=false; 
  int numRows=2, totalRows=2;

  while(true){
    display.clearBuffer();
    display.setFont(u8g2_font_6x12_tr);
    display.setBitmapMode(1);

    display.setCursor(5,12); display.print("V_Sup: "); display.print(velSup);
    display.setCursor(70,12); display.print("ATRAS");
    display.setCursor(5,28); display.print("V_Inf: "); display.print(velInf);
    display.setCursor(70,28); display.print("ENVIAR");

    if(selRow==0 && selCol==0) drawSelector(5,12);
    if(selRow==0 && selCol==1) drawSelector(70,12);
    if(selRow==1 && selCol==0) drawSelector(5,28);
    if(selRow==1 && selCol==1) drawSelector(70,28);

    display.sendBuffer();

    if(!editing){
      if(digitalRead(BTN_ARRIBA)==LOW){ selRow=(selRow-1+totalRows)%totalRows; delay(150);}
      if(digitalRead(BTN_ABAJO)==LOW){ selRow=(selRow+1)%totalRows; delay(150);}
      if(digitalRead(BTN_IZQUIERDA)==LOW){ selCol=(selCol-1+2)%2; delay(150);}
      if(digitalRead(BTN_DERECHA)==LOW){ selCol=(selCol+1)%2; delay(150);}
    } else {
      if(selCol==0){
        if(selRow==0){ if(digitalRead(BTN_ARRIBA)==LOW) velSup=min(velSup+1,100);
                        if(digitalRead(BTN_ABAJO)==LOW) velSup=max(velSup-1,0);}
        if(selRow==1){ if(digitalRead(BTN_ARRIBA)==LOW) velInf=min(velInf+1,100);
                        if(digitalRead(BTN_ABAJO)==LOW) velInf=max(velInf-1,0);}
        delay(100);
      }
    }

    if(digitalRead(BTN_START)==LOW){
      if(selCol==0) editing=!editing;
      else if(selCol==1 && selRow==0) return;
      else if(selCol==1 && selRow==1) sendVelocidad();
      delay(200);
    }
  }
}

// --------------------------------------------------
// Submenú INTERVALO
// --------------------------------------------------
void showSubmenuIntervalo(){
  int selRow=0, selCol=0; bool editing=false; 
  int numRows=1, totalRows=2;

  while(true){
    display.clearBuffer();
    display.setFont(u8g2_font_6x12_tr);
    display.setBitmapMode(1);

    display.setCursor(5,20); display.print("Tiempo:"); display.print(tiempoBol);
    display.setCursor(75,12); display.print("ATRAS");
    display.setCursor(75,28); display.print("ENVIAR");

    if(selRow==0 && selCol==0) drawSelector(5,21);   
    if(selRow==0 && selCol==1) drawSelector(73,13);  
    if(selRow==1 && selCol==1) drawSelector(73,29);  

    display.sendBuffer();

    if(!editing){
      if(digitalRead(BTN_ARRIBA)==LOW){ selRow=(selRow-1+totalRows)%totalRows; delay(150);}
      if(digitalRead(BTN_ABAJO)==LOW){ selRow=(selRow+1)%totalRows; delay(150);}
      if(digitalRead(BTN_IZQUIERDA)==LOW){ selCol=(selCol-1+2)%2; delay(150);}
      if(digitalRead(BTN_DERECHA)==LOW){ selCol=(selCol+1)%2; delay(150);}
    } else {
      if(selCol==0){
        if(selRow==0){ if(digitalRead(BTN_ARRIBA)==LOW) tiempoBol=min(tiempoBol+1,6);
                        if(digitalRead(BTN_ABAJO)==LOW) tiempoBol=max(tiempoBol-1,2);}
        delay(100);
      }
    }

    if(digitalRead(BTN_START)==LOW){
      if(selCol==0) editing=!editing;
      else if(selCol==1 && selRow==0) return;
      else if(selCol==1 && selRow==1) sendIntervalo();
      delay(200);
    }
  }
}

// --------------------------------------------------
// Funciones de envío
// --------------------------------------------------
// --------------------------------------------------
// Funciones de envío optimizadas
// --------------------------------------------------
void sendAngulos() {
  char buffer[32];
  sprintf(buffer, "ANG:%d,%d", angVertical, angHorizontal);
  hc12.println(buffer);
}

void sendEfecto() {
  char buffer[32];
  sprintf(buffer, "EFECTO:%d,%d", efectoLiftado, efectoCortado);
  hc12.println(buffer);
}

void sendVelocidad() {
  char buffer[32];
  sprintf(buffer, "VEL:%d,%d", velSup, velInf);
  hc12.println(buffer);
}

void sendIntervalo() {
  char buffer[32];
  sprintf(buffer, "INTERVALO:%d", tiempoBol);
  hc12.println(buffer);
}

// --------------------------------------------------
// Dibuja el selector (resaltado como cuadro)
// --------------------------------------------------
void drawSelector(int textX, int textY) {
  int midY = textY - 6;          
  display.drawFrame(textX - 6, midY, 2, 2);
}
